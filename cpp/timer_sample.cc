#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


const int ms_wait = 5;
boost::posix_time::ptime first;
boost::posix_time::ptime last = boost::posix_time::microsec_clock::universal_time();

void print(const boost::system::error_code& /*e*/,
    boost::asio::deadline_timer* t, int* count)
{
  int & c = *count;

  if ( c >= 5000 ) return;

  if ( c == 0 )
  {
    first = boost::posix_time::microsec_clock::universal_time();
  }

  boost::posix_time::ptime now = boost::posix_time::microsec_clock::universal_time();

  std::cout << c
	    << " " << (now-last).ticks()
	    << " " << double(c)/((now-first).total_milliseconds()/1000.)
	    << "\n";

  last = now;

  ++c;

  t->expires_at(t->expires_at() + boost::posix_time::millisec(ms_wait));
  t->async_wait(boost::bind(print, boost::asio::placeholders::error, t, count));
}

int main()
{
  boost::asio::io_service io;

  int count = 0;
  boost::asio::deadline_timer t(io, boost::posix_time::millisec(ms_wait));
  t.async_wait(boost::bind(print, boost::asio::placeholders::error, &t, &count));

  io.run();

  std::cout << "Final count is " << count << "\n";

  return 0;
}
