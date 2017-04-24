#include <iostream>
#include <vector>

#define HEAD_PARENT_ID -1
/*
* @params: CST node array, node id(whose subtree contains the child node) and the child node id
* @returns: edge-cut cost
*/
void unPropagate (vector<Node> CST, int nodeId, int childNodeId)
{
  int parentPos = -1, childPos = -1;

  while(nodeId != HEAD_PARENT_ID)
  {
    for(int i = 0; i < CST.size(); i++)
    {
      if(CST[i].userId == nodeId)
        parentPos = i;
      else if(CST[i].userId == childNodeId)
        childPos = i;

      if(parentPos >= 0 && childPos >= 0)
        break;
    }
    CST[parentPos].nodeWeight -= CST[childPos].nodeWeight;
    nodeId = CST[parentPos].parentUserId;
  }
}
