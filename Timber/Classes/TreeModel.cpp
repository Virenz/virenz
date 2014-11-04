#include "TreeModel.h"

static TreeModel* _layer = nullptr;
TreeModel* TreeModel::getInstance()
{
	if (!_layer)
	{
		_layer = new TreeModel();
		_layer->initTree();
		_layer->setScale(0.5f);
	}
	return _layer;
}

void TreeModel::initTree()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	Sprite* base = Sprite::create("base.png");
	this->addChild(base);
	base->setPosition(3, base->getContentSize().height / 2);

	Treebody = Node::create();
	this->addChild(Treebody);
	Treebody->setPositionY(base->getContentSize().height);

	createTreeModel();
	scheduleUpdate();
}

void TreeModel::createTreeModel()
{
	Treebody->removeAllChildren();
	treeQueue.clear();
	treeCache.clear();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	float posY = 0;
	bool isBase = true;
	while (visibleSize.height * 2 > posY)
	{
		auto tree = TreeNode::createNode();
		tree->setBranch(getBranch());
		//	tree is add the Treebody£¬ pos is relative to Treebody's position
		if (isBase)
		{
			posY += tree->getContentSize().height / 2;
			isBase = false;
		}
		else
		{
			posY += tree->getContentSize().height;
		}
		tree->setPositionY(posY);
		Treebody->addChild(tree);
		treeQueue.pushBack(tree);
	}
}

void TreeModel::update(float time)
{
	//	every update£¬ all tree move down relative to the head tree
	for (int idx = 1; idx < treeQueue.size(); idx++)
	{
		auto protree = treeQueue.at(idx - 1);
		auto tree = treeQueue.at(idx);
		tree->setPositionY(protree->getPositionY() + protree->getContentSize().height);
	}
}

TreeNode* TreeModel::deleteTreeHeadNode()
{
	auto tree = treeQueue.at(0);
	treeQueue.erase(0);
	treeCache.pushBack(tree);
	TreeNode* nextTree;
	if (treeCache.size() > 10)
	{
		nextTree = treeCache.at(0);
		Treebody->removeChild(nextTree);	//	remove the delete tree
		treeCache.erase(0);
		nextTree->setPositionX(0);
		nextTree->setRotation3D(Vec3(0, 0, 0));
	}
	else
	{
		nextTree = TreeNode::createNode();
	}
	Treebody->addChild(nextTree);
	nextTree->setBranch(getBranch());
	treeQueue.pushBack(nextTree);
	Treebody->reorderChild(tree, Treebody->getChildrenCount() + 10);	// new add tree is first draw
	auto head = treeQueue.at(0);
	head->runAction(MoveTo::create(0.06f, Vec2(0, tree->getPositionY())));
	return tree;
}

TreeBranchDirection TreeModel::getBranch()
{
	auto isBranch = CCRANDOM_0_1() * 10 < 7;
	if (treeQueue.size() == 0)
	{
		return DEFINE;
	}
	if (!isBranch)
	{
		return DEFINE;
	}
	auto protree = treeQueue.at(treeQueue.size() - 1);
	switch (protree->getHasBranchDirection())
	{
		case LEFT:
			return (CCRANDOM_0_1() * 10 < 5)?DEFINE:LEFT;
			break;
		case RIGHT:
			return (CCRANDOM_0_1() * 10 < 5)?DEFINE:RIGHT;
			break;
		case DEFINE:
			return getAgainBranch();
			break;
		default:
			return DEFINE;
			break;
	}
}

TreeBranchDirection TreeModel::getAgainBranch()
{
	if (treeQueue.size() < 2)
		return DEFINE;
	auto protree = treeQueue.at(treeQueue.size() - 2);
	switch (protree->getHasBranchDirection())
	{
		case LEFT:
			return (CCRANDOM_0_1() * 10 < 6) ? RIGHT : LEFT;
			break;
		case RIGHT:
			return (CCRANDOM_0_1() * 10 < 6) ? LEFT : RIGHT;
			break;
		case DEFINE:
			return (CCRANDOM_0_1() * 10 < 4) ? LEFT : RIGHT;
			break;
		default:
			return DEFINE;
			break;
	}
}

TreeBranchDirection TreeModel::getFirstBranch()
{
	return treeQueue.front()->getHasBranchDirection();
}

void TreeModel::onReset()
{
	createTreeModel();
}