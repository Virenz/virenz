#ifndef __TreeNode__
#define __TreeNode__

#include "cocos2d.h"

USING_NS_CC;

enum TreeBranchDirection
{
	DEFINE,
	LEFT,
	RIGHT
};

class TreeNode : public Node
{
public:
	static TreeNode* createNode();
	void setBranch(TreeBranchDirection enums);
	TreeBranchDirection getHasBranchDirection();

private:
	void initTree();
	TreeBranchDirection enumBranch;

};

#endif // !__TreeNode__
