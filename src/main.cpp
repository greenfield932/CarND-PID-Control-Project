#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>

// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main()
{
  uWS::Hub h;

  // TODO: Initialize the pid variable.
  PID pid;
  pid.Init(0.001, 0.00001, 0.001);


  std::chrono::steady_clock::time_point start, end;
  start = std::chrono::steady_clock::now();

  h.onMessage([&pid, &start, &end](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
          double speed = std::stod(j[1]["speed"].get<std::string>());
          double angle = std::stod(j[1]["steering_angle"].get<std::string>());
          double steer_value;

          /*
          * TODO: Calcuate steering value here, remember the steering value is
          * [-1, 1].
          * NOTE: Feel free to play around with the throttle and speed. Maybe use
          * another PID controller to control the speed!
          */         
          //Measure update time and use it in the PID controller to make sure we do not depend on timings
          end = std::chrono::steady_clock::now();
          double dt = std::chrono::duration<double>(end - start).count();
          start = end;
          std::cout<<"dt:"<<dt<<std::endl;

          double diff_cte = cte - pid.CTE();
          pid.UpdateError(cte);

          //calculate max possible steering, let's assume we do not exceed 10 units cte,
          //diff_cte divided by dt not exceed 2, and take total error value
          double norm = fabs(pid.Kp * 10 + pid.Kd*2 + pid.Ki * fabs(pid.TotalError()));

          //apply PID formula
          steer_value = -pid.Kp * cte - pid.Kd * diff_cte/dt - pid.Ki * pid.TotalError();

          //normalize steering to -1...1
          steer_value = steer_value/norm;

          // DEBUG
          std::cout << "CTE: " << cte << " Steering Value: " << steer_value <<" diff:"<<diff_cte<< std::endl;

          //correct steering if we out of range
          if(steer_value > 1.){
            steer_value = 1.;
          }
          else if(steer_value < -1.){
            steer_value = -1.;
          }

          json msgJson;
          msgJson["steering_angle"] = steer_value;


          //set throttle proportional to 1/cte, the greater error the lower speed
          msgJson["throttle"] = fabs(1./cte);

          //1. example with speed limit
          /*if(speed<40)
                msgJson["throttle"] = fabs(1./cte);
          else
              msgJson["throttle"] = 0;
*/

          //2. max speed example
          //use break on speeds over 50 and
          //if cte is high enough (likely we are on a turn, so cte increase fast)
          if(speed > 50 && fabs(cte) > 1){
            msgJson["throttle"] =-fabs(1./cte);
          }

          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
