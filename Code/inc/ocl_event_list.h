//Copyright (C) 2013 Cem Bassoy.
//
//This file is part of the OpenCL Utility Toolkit.
//
//OpenCL Utility Toolkit is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//OpenCL Utility Toolkit is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with OpenCL Utility Toolkit.  If not, see <http://www.gnu.org/licenses/>.


#ifndef OCL_EVENT_LIST_H
#define OCL_EVENT_LIST_H


#include <vector>
#include <string>
#include <memory>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif


namespace ocl{
class Event;
class Context;

/*! \class EventList ocl_event_list.h "inc/ocl_event_list.h"
  * \brief Aggregation of Event objects
  *
  * An EventList is an aggregation of Event objects. If provided for a
  * command within a command Queue, the command can only execute if
  * all Event s within the EventList are completed. The Event objects correspond to
  * a synchronization points. Be carefull with Memory usage. Only free and
  * reuse Memory objects if the Event objects are completed.
  * The Context of an EventList is determined by its Event objects.Thus,
  * all Event objects must be within the same Context.
  */
class EventList
{
public:
    EventList();
    EventList ( const Event& );
    EventList ( const EventList& );
    ~EventList() = default;

    void       append   ( const Event & event );
    void       append   ( const EventList & other );
    const Event& at       ( size_t index ) const;
    bool       contains ( const Event & event ) const;
    bool       isEmpty  () const;
    void       remove   ( const Event &);
    size_t     size     () const;

    ocl::Context& context() const;

    std::vector<cl_event> events() const;
    void  waitUntilCompleted() const;
    EventList &  operator<< ( const Event & );
    EventList &  operator<< ( const EventList & );
    EventList &  operator=  ( const EventList & );

private:
    std::vector<const ocl::Event*> _events;
    ocl::Context* _ctxt;
};

}

#endif
