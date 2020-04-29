#include <math.h>
#include <uWS/uWS.h>
#include <iostream>
#include <string>
#include <vector>
#include "json.hpp"
#include "PID.h"
#include "Twiddle.h"
// for convenience
using nlohmann::json;
using std::string;
using std::vector;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
string hasData(string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != string::npos) {
    return "";
  }
  else if (b1 != string::npos && b2 != string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main() {
  uWS::Hub h;

  PID pid;


  //  pid.Init(1,1,1);

  //initial guess
   // pid.Init(0.1, 0.00001, 5);

    //twiddle step one
   // pid.Init(0.137263, 9.68619e-06, 4.87291);

    //pid.Init(0.137263, 9.68619e-06, 3);

    pid.Init(0.137263, 9.68619e-06, 15.5215);

    pid.Init(0.137263, 9.68619e-06, 55.5215);

    pid.Init(0.576354, 9.68619e-06, 172.117);

    pid.Init(1.15271, 9.68619e-06, 172.117);

    pid.Init(0.13739, 4.15538e-09, 25.61);

    pid.Init(0.13739, 1.6501e-08, 23.049);

    pid.Init(0.12,0,2.757);

    pid.Init(0.11028, 9.68619e-06, 3.327);
    pid.Init(0.11028, 9.68619e-06, 3.77153);

    /*
     *  New best 0.121308, 9.68619e-06, 3.77153
twiddle
 New best 0.133439, 9.68619e-06, 3.77153
twiddle
 New best 0.146783, 9.68619e-06, 3.77153
twiddle
twiddle
 New best 0.132104, 9.68619e-06, 3.77153
     */

pid.Init(0.132104, 9.68619e-06, 3.77153);
  Twiddle twiddle(pid);

  //
   // pid.Init(17.47891, 18.89687, 11125.80059);
  int iteration = -2;

  bool shouldTwiddle = false;


  h.onMessage([&pid, &twiddle, &iteration, &shouldTwiddle](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length,
                     uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2') {
      auto s = hasData(string(data).substr(0, length));

      if (s != "") {
        auto j = json::parse(s);

        string event = j[0].get<string>();

        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<string>());
          double speed = std::stod(j[1]["speed"].get<string>());
          double angle = std::stod(j[1]["steering_angle"].get<string>());
          pid.UpdateError(cte);
          double steer_value = pid.Run();

          double d_error = pid.GetDError();
       //   std::cout << "pre-steer" << steer_value << std::endl;
          if (steer_value > 1) {
              steer_value = 1;
          } else if (steer_value < -1) {
              steer_value = -1;
          }

          if (iteration++ % 6000 == 0 && shouldTwiddle){
              std::cout << "twiddle " << std::endl;
              twiddle.Execute();
          }

          
          // DEBUG
//         std::cout << "CTE: " << cte << " Steering Value: " << steer_value
//                    << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steer_value;

        //  std::cout << cte << std::endl;

          double throttle = 1;
      
          if (d_error > 0 || cte > 0.5){
              if (speed < 15) {
                  throttle = 0.3;
              } else {
                  throttle = 0;
              }
          }
          msgJson["throttle"] = throttle;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
        //  std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }  // end "telemetry" if
      } else {
        // Manual driving
        string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }  // end websocket message if
  }); // end h.onMessage

  h.onConnection([&h, &iteration, &twiddle](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
     // auto p = twiddle.GetCurrentBest();
    //  pid.Init(p[0], p[1], p[2]);

    if (iteration > 0){
        twiddle.Execute();
    }

      iteration = 1;
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, 
                         char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port)) {
    std::cout << "Listening to port " << port << std::endl;
  } else {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  
  h.run();
}