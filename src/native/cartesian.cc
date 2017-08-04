#include "cartesian.hpp"

namespace Cartesian {

double toRadians(const double &degrees) { return degrees / 180 * PI; }

void distance(const v8::FunctionCallbackInfo<v8::Value> &args) {
  v8::Isolate *isolate = args.GetIsolate();

  // get args
  v8::Local<v8::Array> _points = v8::Local<v8::Array>::Cast(args[0]);
  v8::Local<v8::Array> _center = v8::Local<v8::Array>::Cast(args[1]);
  const char units = (char)(args[2]->Uint32Value());

  const unsigned int length = _points->Length();

  // pass locations to C++ array
  double points[length][2];
  for (unsigned int i = 0; i < length; ++i) {
    v8::Local<v8::Array> _element = v8::Local<v8::Array>::Cast(_points->Get(i));
    points[i][0] = _element->Get(0)->NumberValue();
    points[i][1] = _element->Get(1)->NumberValue();
  }
  double center[2];
  {
    v8::Local<v8::Array> _centerElement = v8::Local<v8::Array>::Cast(_center);
    center[0] = _centerElement->Get(0)->NumberValue();
    center[1] = _centerElement->Get(1)->NumberValue();
  }

  // record distances from each location to center
  v8::Local<v8::Array> distances = v8::Array::New(isolate);
  for (unsigned int i = 0; i < length; ++i) {
    // latitudes
    const double φ1 = toRadians(center[0]);
    const double φ2 = toRadians(points[i][0]);

    // dLat, dLng
    const double Δφ = toRadians(points[i][0] - center[0]);
    const double Δλ = toRadians(points[i][1] - center[1]);

    // Haversine formula:
    // a = sin²(Δφ/2) + cos φ1 ⋅ cos φ2 ⋅ sin²(Δλ/2)
    // c = 2 ⋅ atan2( √a, √(1−a) )
    // d = R ⋅ c
    const double a =
        std::sin(Δφ / 2) * std::sin(Δφ / 2) +
        std::cos(φ1) * std::cos(φ2) * std::sin(Δλ / 2) * std::sin(Δλ / 2);
    const double c = 2 * std::atan2(sqrt(a), std::sqrt(1 - a));
    const double d =
        RADIUS_METERS * c * (units == 'm' ? METERS_TO_MILES : METERS_TO_KM);

    distances->Set(i, v8::Number::New(isolate, d));
  }

  // create object to hold results
  v8::Local<v8::Object> result = v8::Object::New(isolate);
  result->Set(v8::String::NewFromUtf8(isolate, "origins"), _points);
  result->Set(v8::String::NewFromUtf8(isolate, "destination"), _center);
  result->Set(v8::String::NewFromUtf8(isolate, "distances"), distances);

  args.GetReturnValue().Set(result);
}

void init(v8::Local<v8::Object> exports) {
  NODE_SET_METHOD(exports, "distance", distance);
}

NODE_MODULE(addon, init);

} // namespace Cartesian
