/* Copyright (c) 2015, Stefan Isler, islerstefan@bluewin.ch
*
This file is part of ig_active_reconstruction, a ROS package for...well,

ig_active_reconstruction is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
ig_active_reconstruction is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public License
along with ig_active_reconstruction. If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdexcept>

#include "ig_active_reconstruction_ros/robot_ros_client_ci.hpp"
#include "ig_active_reconstruction_ros/conversions.hpp"

#include "ig_active_reconstruction_msgs/ViewRequest.h"
#include "ig_active_reconstruction_msgs/RetrieveData.h"
#include "ig_active_reconstruction_msgs/MovementCostCalculation.h"
#include "ig_active_reconstruction_msgs/MoveToOrder.h"


namespace ig_active_reconstruction
{
  
namespace robot
{
  
  RosClientCI::RosClientCI( ros::NodeHandle nh_sub )
  : nh_sub_(nh_sub)
  {
    current_view_retriever_ = nh_sub_.serviceClient<ig_active_reconstruction_msgs::ViewRequest>("robot/current_view");
    data_retriever_ = nh_sub_.serviceClient<ig_active_reconstruction_msgs::RetrieveData>("robot/retrieve_data");
    cost_retriever_ = nh_sub_.serviceClient<ig_active_reconstruction_msgs::MovementCostCalculation>("robot/movement_cost");
    robot_mover_ = nh_sub_.serviceClient<ig_active_reconstruction_msgs::MoveToOrder>("robot/move_to");
  }
  
  views::View RosClientCI::getCurrentView()
  {
    ig_active_reconstruction_msgs::ViewRequest request;
    bool response = current_view_retriever_.call(request);
    
    if( !response )
      throw std::runtime_error("RosClientCI::getCurrentView failed for unknown reason.");
    
    return ros_conversions::viewFromMsg(request.response.view);
  }
  
  RosClientCI::ReceptionInfo RosClientCI::retrieveData()
  {
    ig_active_reconstruction_msgs::RetrieveData request;
    bool response = data_retriever_.call(request);
    
    if( !response )
      throw std::runtime_error("RosClientCI::retrieveData failed for unknown reason.");
    
    return ros_conversions::robotReceptionInfoFromMsg(request.response.receive_info);
  }
  
  MovementCost RosClientCI::movementCost( views::View& target_view )
  {
    ig_active_reconstruction_msgs::MovementCostCalculation request;
    views::View current_view = getCurrentView();
    
    request.request.start_view = ros_conversions::viewToMsg(current_view);
    request.request.target_view = ros_conversions::viewToMsg(target_view);
    request.request.additional_information = true;
    
    bool response = cost_retriever_.call(request);
    
    MovementCost cost = ros_conversions::movementCostFromMsg(request.response.movement_cost);
    if( !response )
    {
      cost.exception = MovementCost::Exception::RECEPTION_FAILED;
    }
    
    return cost;
  }
  
  MovementCost RosClientCI::movementCost( views::View& start_view, views::View& target_view, bool fill_additional_information  )
  {
    ig_active_reconstruction_msgs::MovementCostCalculation request;
    
    request.request.start_view = ros_conversions::viewToMsg(start_view);
    request.request.target_view = ros_conversions::viewToMsg(target_view);
    request.request.additional_information = fill_additional_information;
    
    bool response = cost_retriever_.call(request);
    
    MovementCost cost = ros_conversions::movementCostFromMsg(request.response.movement_cost);
    if( !response )
    {
      cost.exception = MovementCost::Exception::RECEPTION_FAILED;
    }
    
    return cost;
  }
  
  bool RosClientCI::moveTo( views::View& target_view )
  {
    ig_active_reconstruction_msgs::MoveToOrder request;
    request.request.target_view = ros_conversions::viewToMsg(target_view);
    
    bool response = robot_mover_.call(request);
    if( !response )
    {
      return false;
    }
    
    return request.response.success;
  }
  
  
}

}