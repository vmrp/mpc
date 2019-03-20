/**
 ** filelist.c the implement of filelist.h
 ** 
 ** 文件列表 链表操作封装 方法
 **
 ** 注：1.列表序号从1开始
 ** 
 ** @Author Jianbin Zhu 2011/9/13
 **/

#include "filelist.h"
#include "mrc_base.h"


static struct 
{
	FILE head;	//固定链表头结点
	PFILE pEnd; //总是指向链表最后一个节点

	int size;	//总数
	int fc, dc;	//文件数、目录数
	boolean b_dirFirst;	//排序目录靠前
	boolean b_order_ascending;	//升序排序
	int i_order_by;	//排序条件

	/**
	 * 为了避免每次访问某个元素的时候都从第一个开始找
	 * 增加这两个参数，
	 */
	PFILE pCur;		//总是指向当前访问的节点
	int cur_index;	//当前访问的节点的序号 从1开始
}listData;


//释放一个文件实体
#define F_free(pf) \
	do { \
		if(pf) \
		{	\
			if(pf->name) \
				free(pf->name); \
			free(pf); \
			pf = NULL; \
		}	\
	} while (0);


void FL_init(boolean dirFirst, boolean order_ascending, int order_by)
{
	memset(&listData, 0, sizeof(listData));
	listData.head.prev = NULL;
	listData.head.next = NULL;
	listData.pEnd = &listData.head;
	listData.b_dirFirst = dirFirst;
	listData.b_order_ascending = order_ascending;
	listData.i_order_by = order_by;
}

PFILE F_new(char *name, int nameLen, boolean isFile, int format, boolean isHiden)
{
	PFILE tmp;
	char *str;
	static int i;

	if(!name) return NULL;

	tmp = (PFILE)malloc(sizeof(FILE));
	if(!tmp) return NULL;

	str = (char *)malloc(nameLen + 1);
	if(!str) {free(tmp); return NULL;};
	memcpy(str, name, nameLen);
	str[nameLen] = 0;

	tmp->b_checked = false;
	tmp->b_isFile = isFile;
	tmp->b_isHiden = isHiden;
	tmp->format = format;
	tmp->length = -2;	//默认初始化为-2，说明长度未初始化
	tmp->next = NULL;
	tmp->prev = NULL;
	tmp->name = str;

	mrc_printf("new file success %d !", ++i);
	//计数
	if(isFile) listData.fc++;
	else listData.dc++;

	return tmp;
}

void FL_add(PFILE pf)
{
	if(!pf) return;

	listData.pEnd->next = pf;
	listData.size++;
	listData.pEnd = pf;
}


/**
 * \通过特定条件比较两个文件：
 * 若 scr 比 dst 大：返回 >0
 * 若 src 比 dst 小：返回 <0
 * 若 src = dst ：返回 =0
 **/
static int F_cmp(FILE_CmpCond by, PFILE src, PFILE dst)
{
	switch(by)
	{
	case CMPBY_NAME:	//按名称比较
		{
			int a, b;

			//获得首字母
			a = (uint8)src->name[0]; 
			if(IS_SMLETTER(a)) a -= 32;

			b = (uint8)dst->name[0]; 
			if(IS_SMLETTER(b)) b -= 32;

			return a-b;
		}
		break;

	case CMPBY_DATE:	//按日期比较
		{

		}
		break;

	case CMPBY_SIZE:	//按大小比较
		{

		}
		break;
	}

	return 0;
}

//节点tmp，前插入节点pf
static void FL_insertBefore(PFILE pf, PFILE tmp)
{
	tmp->prev->next = pf;
	pf->prev = tmp->prev;
	pf->next = tmp;
	tmp->prev = pf;
}

//添加并按名称排序
void FL_add_sortByName(PFILE pf)
{
	PFILE tmp;

	if(!pf) return;

	listData.size++;

	//第一个节点
	if(listData.size == 1) 
	{
		listData.pEnd->next = pf;
		pf->prev = listData.pEnd;
		listData.pEnd = pf;

		return;	//只有一个文件不排序
	}

	//排序插入
	tmp = listData.head.next;
	while(tmp)
	{
		//先考虑文件，先默认目录在前
		if(pf->b_isFile)	//待插入的是文件
		{
			//略过文件夹
			if(!tmp->b_isFile) goto NEXT;

			//到了文件列表处
			if(listData.b_order_ascending)	//升序排序
			{
				/**
				 * 插入条件：待插节点 <= 当前节点
				 * 插入位置：在当前节点前插入
				 */
				if(F_cmp(listData.i_order_by, pf, tmp) <= 0)
				{
					FL_insertBefore(pf, tmp);

					//找到了插入位置，则直接跳出循环
					break;
				}
			}else{	//降序排序
				/**
				 * 插入条件：当前节点的下一节点不为NULL，待插节点 >= 当前节点的下一节点
				 * 插入位置：当前节点后插入
				 */
				if(tmp->next && F_cmp(listData.i_order_by, pf, tmp->next) >= 0)
				{
					FL_insertBefore(pf, tmp->next);

					//找到了插入位置，则直接跳出循环
					break;
				}
			}
			
		}else{	//待插入的是文件夹
			//tmp 不是文件夹的情况：1.还没有文件夹 2.所有文件夹找完了，都是插在当前节点tmp前
			if(tmp->b_isFile)
			{
				FL_insertBefore(pf, tmp);

				//找到了插入位置，则直接跳出循环
				break;
			}

			if(listData.b_order_ascending)	//升序排序
			{
				if(F_cmp(listData.i_order_by, pf, tmp) <= 0)
				{
					FL_insertBefore(pf, tmp);

					//找到了插入位置，则直接跳出循环
					break;
				}
			}else{
				if(tmp->next && F_cmp(listData.i_order_by, pf, tmp->next) >= 0)
				{
					FL_insertBefore(pf, tmp->next);

					//找到了插入位置，则直接跳出循环
					break;
				}
			}
		}

NEXT:	//跳到下一个
		/**
		 * 当移动到最后一个节点还是不符合插入条件则他就是最后一个，
		 * 这里跳出循环tmp就是NULL了，该节点插在最后节点 listData.pEnd 后即可
		 */
		tmp = tmp->next;
	}

	//检测 tmp，如果是NULL则说明没插入，则插在最后
	if(tmp == NULL)
	{
		listData.pEnd->next = pf;
		pf->prev = listData.pEnd;
		listData.pEnd = pf;
	}
}

//列表清空
void FL_clear(void)
{
	PFILE tmp = listData.head.next;

	while(tmp)
	{
		PFILE del = tmp;
		tmp = tmp->next;
		if(del->name) free(del->name);
		free(del);
	}

	listData.head.next = NULL;
	listData.pEnd = &listData.head;
	listData.size = listData.dc = listData.fc = 0;

	listData.cur_index = 0;
	listData.pCur = NULL;
}

PFILE FL_get(int index)	//序号从1开始
{
	int i;
	PFILE tmp = &listData.head;

	if(index < 1 || index > listData.size)
		return NULL;

	//pCur未被初始化，从开头找
	if(listData.cur_index == 0 || listData.pCur == NULL)
	{
		for(i=0; i<index; i++)
		{
			if(!tmp->next)
			{
				mrc_printf("NULL nodal at index %d", i+2);
				return NULL;
			}
			tmp = tmp->next;
		}
	}else	//从 pCur 位置开始搜寻
	{
		/*--20111022添加-- 判断要找的index距哪头最近
		 *开头、结尾、当前点
		 */
		int d0, d1;

		while(listData.cur_index != index)
		{
			if(listData.cur_index < index)	//前进
			{
				d0 = listData.size - index;	//距尾节点
				d1 = index - listData.cur_index;

				if(d0 < d1){	//距尾节点更近，从尾节点找
					listData.cur_index = listData.size;
					listData.pCur = listData.pEnd;
					continue;
				}

				listData.cur_index++;
				listData.pCur = listData.pCur->next;
			}else{	//后退
				d0 = index - 1;	//距开头
				d1 = listData.cur_index - index;	//距当前

				if(d0 < d1){	//距开头更近，从开头找
					listData.cur_index = 1;
					listData.pCur = listData.head.next;
					continue;
				}

				listData.cur_index--;
				listData.pCur = listData.pCur->prev;
			}
		}

		tmp = listData.pCur;
	}
	
	return tmp;
}

char* FL_getName(int index)
{
	return FL_get(index)->name;
}
FILE_Format FL_getFormat(int index)
{
	return FL_get(index)->format;
}
int FL_getFCount(void)
{
	return listData.fc;
}
int FL_getDCount(void)
{
	return listData.dc;
}
boolean FL_isChecked(int index)
{
	return FL_get(index)->b_checked;
}
boolean FL_isFile(int index)
{
	return FL_get(index)->b_isFile;
}
boolean FL_isHiden(int index)
{
	return FL_get(index)->b_isHiden;
}

//内部方法：比较两个文件实体
static boolean F_equal(PFILE src, PFILE dst)
{
	//目前只对文件名判断
	return (0 == strcmp(src->name, dst->name));
}

int FL_indexOf(PFILE pf)
{
	int i = 1;
	PFILE tmp = listData.head.next;

	while(tmp)
	{
		if (F_equal(tmp, pf))
		{
			return i;
		}
		i++;
		tmp = tmp->next;
	}
	//出了循环说明文件不存在
	return -1;
}

boolean FL_isEmpty(void)
{
	return (listData.size == 0);
}

void FL_check(int index, boolean checked)
{
	PFILE tmp = FL_get(index);

	if(!tmp)
	{
		mrc_printf("FL_check: NULL nodal");
		return;
	}

	tmp->b_checked = checked;
}

int FL_size(void)
{
	return listData.size;
}

//节点替换
void FL_set(int index, PFILE pf)
{
	PFILE tmp = FL_get(index);

	if(!tmp || !pf)
	{
		mrc_printf("FL_set: NULL nodal");
		return;
	}

	//最后一个节点
	if(index == listData.size)
	{
		pf->prev = tmp->prev;
		tmp->prev->next = pf;
		F_free(tmp);
	}else{
		tmp->prev->next = pf;
		pf->prev = tmp->prev;
		tmp->next->prev = pf;
		pf->next = tmp->next;
		F_free(tmp);
	}
}

void FL_remove(int index)
{
	PFILE tmp = FL_get(index);

	if(!tmp)
	{
		mrc_printf("FL_remove: NULL nodal");
		return;
	}
	
	//计数器
	if(tmp->b_isFile) listData.fc--;
	else listData.dc--;
	listData.size--;

	//判断pcur
	if(tmp == listData.pCur)
	{
		listData.cur_index = 0;
		listData.pCur = NULL;
	}

	//删除最后一个节点特殊，其他一致
	if(index == listData.size)
	{
		tmp->prev->next = NULL;
		F_free(tmp);
	}else{
		tmp->prev->next = tmp->next;
		tmp->next->prev = tmp->prev;
		F_free(tmp);
	}
}