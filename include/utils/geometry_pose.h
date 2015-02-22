/* Copyright (c) 2015, Stefan Isler, islerstefan@bluewin.ch
*
* 
geometry_pose is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
geometry_pose is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public License
along with geometry_pose. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <Eigen/Geometry>
#include <Eigen/StdVector>
#include "utils/relative_movement"

namespace st_is
{
  /** class to represent a geometry pose in 3d space */
  class GeometryPose
  {
  public:
    Eigen::Vector3d position;
    Eigen::Quaterniond orientation;
    
    /** executes a relative movement on the pose */
    GeometryPose operator+( st_is::RelativeMovement&  _second );
  };
  
}