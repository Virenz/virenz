#include "TreeNode.h"

TreeNode* TreeNode::createNode()
{
	auto treeNode = new TreeNode();
	treeNode->initTree();
	return treeNode;
}

void TreeNode::initTree()
{
	Sprite* body = Sprite::create("body.jpg");
	this->addChild(body, 0, "body");
	Sprite* branch = Sprite::create("branch.png");
	this->addChild(branch, 1, "branch");
	this->setContentSize(Size(body->getContentSize().width,
		body->getContentSize().height));
}

void TreeNode::setBranch(TreeBranchDirection enums)
{
	enumBranch = enums;
	auto branch = this->getChildByName("branch");
	auto body = this->getChildByName("body");
	branch->setVisible(enums != DEFINE);		//enums £¡= DEFINE³ÉÁ¢·µ»Ø1

	if (enums == DEFINE)
	{
		return;
	}
	if (enums == RIGHT)
	{
		branch->setScaleX(1);
		branch->setPositionX(body->getContentSize().width);
	}
	else
	{
		branch->setScaleX(-1);
		branch->setPositionX(-body->getContentSize().width);
	} 
}

TreeBranchDirection TreeNode::getHasBranchDirection()
{
	return enumBranch;
}