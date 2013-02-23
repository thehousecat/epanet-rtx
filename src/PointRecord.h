//
//  PointRecord.h
//  epanet-rtx
//
//  Created by the EPANET-RTX Development Team
//  See README.md and license.txt for more information
//  

#ifndef epanet_rtx_PointRecord_h
#define epanet_rtx_PointRecord_h

// basics
#include <string>
#include <vector>
#include <deque>
#include <fstream>

#include "Point.h"
#include "rtxMacros.h"
#include "rtxExceptions.h"

#include <boost/circular_buffer.hpp>
#include <boost/signals2/mutex.hpp>

using std::string;

namespace RTX {
  
  /*! 
   \class PointRecord
   \brief A Point Record Class for storing and retrieving Points.
   
   The base PointRecord class works with the PersistentContainer class to store arbitrary point data for arbitrary identifiers. Derive to add specific persistence implementations
   */
  
  /*! 
   \fn virtual Point PointRecord::point(const std::string &name, time_t time) 
   \brief Get a Point with a specific name at a specific time.
   \param name The name of the data source (tag name).
   \param time The requested time.
   \return The requested Point (as a shared pointer).
   \sa Point
   */
  /*! 
   \fn std::vector<Point> PointRecord::pointsInRange(const std::string &name, time_t startTime, time_t endTime) 
   \brief Get a vector of Points with a specific name within a specific time range.
   \param name The name of the data source (tag name).
   \param startTime The beginning of the requested time range.
   \param endTime The end of the requested time range.
   \return The requested Points (as a vector of shared pointers)
   \sa Point
   */
  /*! 
   \fn std::vector<Point> PointRecord::pointsAdjacentTo(const std::string &name, time_t time) 
   \brief Get a vector of Points with a specific name adjacent to a specified time.
   \param name The name of the data source (tag name).
   \param time The requested time.
   \return The requested Points (as a vector of shared pointers)
   \sa Point
   */
  
  class PointRecord {
    
  public:
    RTX_SHARED_POINTER(PointRecord);
    PointRecord();
    virtual ~PointRecord() {};
    
    virtual std::string registerAndGetIdentifier(std::string recordName);    // registering record names.
    virtual std::vector<std::string> identifiers();
    virtual void preFetchRange(const string& identifier, time_t start, time_t end);   // prepare to retrieve range of values
    
    virtual bool isPointAvailable(const string& identifier, time_t time);
    virtual Point point(const string& identifier, time_t time);
    virtual Point pointBefore(const string& identifier, time_t time);
    virtual Point pointAfter(const string& identifier, time_t time);
    virtual std::vector<Point> pointsInRange(const string& identifier, time_t startTime, time_t endTime);
    virtual void addPoint(const string& identifier, Point point);
    virtual void addPoints(const string& identifier, std::vector<Point> points);
    virtual void reset();
    virtual void reset(const string& identifier);
    virtual Point firstPoint(const string& id);
    virtual Point lastPoint(const string& id);
    virtual std::string name();
    
    virtual void setName(const std::string& name);
    void setConnectionString(const std::string& connection);
    const std::string& connectionString();
    virtual void connect() throw(RtxException){};
    virtual bool isConnected(){return true;};
    
    virtual std::ostream& toStream(std::ostream &stream);
    
    // types
    typedef std::pair<double,double> PointPair_t;
    typedef std::pair<time_t, PointPair_t > TimePointPair_t;
    typedef boost::circular_buffer<TimePointPair_t> PointBuffer_t;
    typedef std::pair<PointBuffer_t, boost::shared_ptr<boost::signals2::mutex> > BufferMutexPair_t;
    typedef std::map<std::string, BufferMutexPair_t> KeyedBufferMutexMap_t;
    
    size_t _defaultCapacity;
    
  protected:
    std::string _cachedPointId;
    Point _cachedPoint;
    
  private:
    std::map<std::string, BufferMutexPair_t > _keyedBufferMutex;
    std::string _connectionString;
    std::string _name;
    Point makePoint(PointBuffer_t::iterator iterator);
  };
  
  std::ostream& operator<< (std::ostream &out, PointRecord &pr);

}

#endif
