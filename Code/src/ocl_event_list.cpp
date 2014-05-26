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

#include <ocl_event.h>
#include <ocl_event_list.h>
#include <ocl_query.h>
#include <ocl_context.h>


#include <utl_assert.h>

#include <algorithm>


/*! \brief Instantiates an EventList. */
ocl::EventList::EventList () :
    _events(), _ctxt(0)
{
}

/*! \brief Instantiates an EventList.
  *
  * \param event is an Event inserted the list.
  */
ocl::EventList::EventList ( const ocl::Event & event ) :
    _events(1,&event), _ctxt(&event.context())
{
    TRUE_ASSERT(_ctxt != 0, "Context not valid.");
}

/*! \brief Instantiates an EventList.
  *
  * \param other is an EventList inserted the list.
  */
ocl::EventList::EventList ( const EventList & other ) :
    _events(other._events), _ctxt(0)
{
    const ocl::Event *efirst = _events.front();
    _ctxt = &efirst->context();
    TRUE_ASSERT(_ctxt != 0, "Context not valid.");
}

/*! \brief Appends an Event to this EventList.
  *
  * Context of the Event and EventList must be the same.
  *
  * \param event to be inserted into the EventList.
  *
  * \warning The event object must still be in scope!!!!
  */
void ocl::EventList::append ( const ocl::Event & event )
{
    if(this->_events.empty()) {
        _ctxt = &event.context();
        TRUE_ASSERT(_ctxt != 0, "Context not valid");
    }
    this->_events.push_back(&event);

}

/*! \brief Appends an EventList to this EventList.
  *
  * Context of this EventList and the appended EventList must be the same.
  *
  * \param other to be inserted into the EventList.
  */
void ocl::EventList::append ( const ocl::EventList & other )
{
    for(auto it = other._events.begin(); it != other._events.end(); ++it)
    {
        const ocl::Event *e = *it;
        TRUE_ASSERT(*_ctxt == e->context(), "Context not valid.");
    }
    this->_events.insert(this->_events.end(), other._events.begin(), other._events.end());
}

/*! \brief Returns the specified Event. */
const ocl::Event& ocl::EventList::at ( size_t index ) const
{
    return *this->_events.at(index);
}

/*! \brief Returns true if the specified Event is within this EventList. */
bool ocl::EventList::contains ( const ocl::Event & event ) const
{
    return (std::find(this->_events.begin(), this->_events.end(), &event) != this->_events.end());
}

/*! \brief Returns true if this EventList is empty. */
bool ocl::EventList::isEmpty () const
{
    return this->_events.empty();
}

/*! \brief Returns the specified Event from this EventList. */
void ocl::EventList::remove ( const ocl::Event & event)
{
    auto it = std::find(this->_events.begin(), this->_events.end(), &event);
    if(it == this->_events.end()) return;
    this->_events.erase(it);
}

/*! \brief Returns the number of Event objects within this EventList. */
size_t ocl::EventList::size () const
{
    return this->_events.size();
}

/*! \brief Returns the Context of this EventList.
  *
  * The Context of the EventList corresponds to
  * the Context of all Event objects within this
  * EventList.
  */
ocl::Context& ocl::EventList::context() const
{
    return *this->_ctxt;
}

/*! \brief Waits until all Event objects within this EventList are completed. */
void ocl::EventList::waitUntilCompleted() const
{
    for(const ocl::Event *event : this->_events)
        event->waitUntilCompleted();
}

/*! \brief Appends the Event into this EventList.
  *
  * \returns this EventList
*/
ocl::EventList & ocl::EventList::operator<< ( const ocl::Event & event )
{
    this->append(event);
	return *this;
}

/*! \brief Appends the Event into this EventList.
  *
  * \returns this EventList
  */
ocl::EventList & ocl::EventList::operator<< ( const EventList & other )
{
    this->append(other);
	return *this;
}

/*! \brief Appends the Event into this EventList.
  *
  * \returns this EventList
  */
ocl::EventList & ocl::EventList::operator= ( const EventList & other )
{
    _ctxt = other._ctxt;
    this->_events.assign(other._events.begin(), other._events.end());
	return *this;
}

/*! \brief Returns the EventList with OpenCL events
  *
  * \returns this EventList with OpenCL events
  */
std::vector<cl_event> ocl::EventList::events() const
{
    std::vector<cl_event> e;
    for(const ocl::Event *event : this->_events){
        e.push_back(event->id());
    }
	return e;
}
