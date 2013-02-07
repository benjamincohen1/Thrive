#include <vector>
#include <sstream>

#include "Engine.h"

Engine::Engine()
{
    //nodeMap;
}

void Engine::addEntity(Entity* entity)
{
    entityList.insert(entityList.end(),entity);
    //Here we have to see what components the entity contains, create nodes
    //containing them and add the nodes to the nodeLists
    //For now, just create a MoveNode. This should be done dinamically
    if (entity->has({"OgreNode","Velocity"}))
    {
        MoveNode* moveNode = new MoveNode();
        moveNode->ogreNode = (OgreNodeComponent*) entity->get("OgreNode");
        moveNode->velocity = (VelocityComponent*) entity->get("Velocity");
        std::vector<Node*>* nodeList = getNodeList("Move");
        nodeList->push_back(moveNode);
    }
    
    if (entity->has({"Agent","Velocity"}))
    {
        ControllerNode* controllerNode = new ControllerNode();
        controllerNode->velocity = (VelocityComponent*) entity->get("Velocity");
        controllerNode->agent = (AgentComponent*) entity->get("Agent");
        std::vector<Node*>* nodeList = getNodeList("Controller");
        nodeList->push_back(controllerNode);
    }
    
}

void Engine::removeEntity(Entity* entity)
{
    //This code is just horrible. A different implementation of entityList is needed
    for (std::vector<Entity*>::iterator i = entityList.begin();i!=entityList.end();i++){
        if ((*i) == entity)
            entityList.erase(i);
            break;
    }
    
}

void Engine::addSystem(ISystem* system)
{
    systemList.insert(systemList.end(),system);
}

void Engine::removeSystem(ISystem* system)
{
    //This code is just horrible. A different implementation of systemList is needed
    for (std::vector<ISystem*>::iterator i = systemList.begin();i!=systemList.end();i++){
        if ((*i) == system)
            systemList.erase(i);
            break;
    }
}

std::vector<Node*>* Engine::getNodeList(std::string node)
{
    if (nodeMap.count(node)==0)
    {
        // the [] operator is suposed to create a new element if the key passed doesen't match any result
        // but whatever it returns i can't call any vector's method on.
        std::pair<std::string,std::vector<Node*>*> p = std::pair<std::string,std::vector<Node*>*>(node,new std::vector<Node*>());
        nodeMap.insert(p);
    }
    return nodeMap[node];
}

void Engine::update(Ogre::Real deltaTime)
{
    for (std::vector<ISystem*>::iterator i = systemList.begin(); i!=systemList.end(); i++)
    {
        (*i)->update(deltaTime);
    }
}