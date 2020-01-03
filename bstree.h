#ifndef __BSTREE_H
#define __BSTREE_H


/**
 * ע��ӿ���������
 * bstree.c�ļ��ײ��Ľӿ�����Ҫʵ���ڴ�������ͷź�����������ʵ�ַ�������Ҫע���������
 * bstree.c�ļ��еĽӿ�����ʵ��ֻ��һ��ʾ��������C���malloc��free������ʵ�֣�ʹ�����ǻ�Ҫ #include <stdlib.h>
 */

/**
 * ����ע������
 * 1. ���ļ���BSTreeNode_t����Ȼ������parent������ʵ���в�û�в������������
 * �����Ҫ�����˱��������޸�bstree.c�е���غ������������Ҫ�˱�����������BSTreeNode_t�Ķ����н���ע�͵�
 * 2. API��ʹ���˵ݹ飬��������Ķ�ջ��С�����ս����ͬ��API�ݹ����Ȳ�һ�����Զ�ջ��С��Ҫ��Ҳ��һ��
 * 3. API��������ֶ�ջ�������������������ջ���������ᷢ��������Ԥ֪�Ĵ���
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
 * ��������������
 */
BSTree_t BSTreeCreate();

/**
 * ɾ������������
 */
void BSTreeDelete(BSTree_t bst);

/**
 * �����㵽������������
 */
void BSTreeInsert(BSTree_t bst, BSTreeNodeCompareFunc_t compare, void *data);

/**
 * ɾ�����Ӷ�����������
 * ����ֵ  0-ʧ�ܣ���ƥ�������bstΪNULL��  1-�ɹ�
 */
int BSTreeRemove(BSTree_t bst, BSTreeNodeCompareFunc_t compare, void* data);

/**
 * �ڶ����������в���Ԫ��
 * ����ֵ  NULL-ʧ�ܣ�δ�ҵ�ƥ�������bstΪNULL��
 */
void* BSTreeSearch(BSTree_t bst, BSTreeNodeCompareFunc_t compare, void* data);

/**
 * �������������
 */
void BSTreeInOrder(BSTree_t bst, BSTreeNodeVisitFunc_t visit);

/**
 * ���������������
 */
void BSTreeReInOrder(BSTree_t bst, BSTreeNodeVisitFunc_t visit);

/**
 * ��ӡ������
 */
void BSTreePrint(BSTree_t bst, BSTreeNodeVisitFunc_t visit);


#endif
