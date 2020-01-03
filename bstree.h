#ifndef __BSTREE_H
#define __BSTREE_H


/**
 * 注意接口区的内容
 * bstree.c文件首部的接口区需要实现内存申请和释放函数，不仅是实现方法，还要注意参量类型
 * bstree.c文件中的接口区的实现只是一个示例，它用C库的malloc和free函数来实现，使用它们还要 #include <stdlib.h>
 */

/**
 * 其它注意事项
 * 1. 此文件在BSTreeNode_t中虽然定义了parent，但是实现中并没有操作这个变量，
 * 如果需要操作此变量，请修改bstree.c中的相关函数。如果不需要此变量，可以在BSTreeNode_t的定义中将其注释掉
 * 2. API中使用了递归，这会对任务的堆栈大小提出挑战，不同的API递归的深度不一样，对堆栈大小的要求也不一样
 * 3. API不允许出现堆栈不足的情况，否则任务堆栈会溢出，这会发生不可以预知的错误
 */



typedef int (*BSTreeNodeCompareFunc_t)(void* dataNode, void* dataUser);

typedef void (*BSTreeNodeVisitFunc_t)(void* data);


struct bstree_node
{
    struct bstree_node* left;
    struct bstree_node* right;
    struct bstree_node* parent;
    void* data;
};
typedef struct bstree_node BSTreeNode_t;
typedef struct bstree_node* BSTree_t;


/**
 * 创建二叉排序树
 */
BSTree_t BSTreeCreate();

/**
 * 删除二叉排序树
 */
void BSTreeDelete(BSTree_t bst);

/**
 * 插入结点到二叉排序树中
 */
void BSTreeInsert(BSTree_t bst, BSTreeNodeCompareFunc_t compare, void *data);

/**
 * 删除结点从二叉排序树中
 * 返回值  0-失败（无匹配结点或者bst为NULL）  1-成功
 */
int BSTreeRemove(BSTree_t bst, BSTreeNodeCompareFunc_t compare, void* data);

/**
 * 在二叉排序树中查找元素
 * 返回值  NULL-失败（未找到匹配结点或者bst为NULL）
 */
void* BSTreeSearch(BSTree_t bst, BSTreeNodeCompareFunc_t compare, void* data);

/**
 * 中序遍历二叉树
 */
void BSTreeInOrder(BSTree_t bst, BSTreeNodeVisitFunc_t visit);

/**
 * 逆中序遍历二叉树
 */
void BSTreeReInOrder(BSTree_t bst, BSTreeNodeVisitFunc_t visit);

/**
 * 打印二叉树
 */
void BSTreePrint(BSTree_t bst, BSTreeNodeVisitFunc_t visit);


#endif
