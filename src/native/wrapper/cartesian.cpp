#include "../cartesian.h"
#include <node.h>

namespace Cartesian
{
/**
 * Calculates the Cartesian (Earthly) distance between two Lat/Lng points.
 */
void distance(const v8::FunctionCallbackInfo<v8::Value> & args)
{
  v8::Isolate * isolate = args.GetIsolate();

  // get args
  v8::Local<v8::Array> _points = v8::Local<v8::Array>::Cast(args[0]);
  v8::Local<v8::Array> _center = v8::Local<v8::Array>::Cast(args[1]);
  const char           unit    = (char)(args[2]->Uint32Value());

  const size_t length = _points->Length();

  // pass locations to C++ array
  double points[length][2];
  for (size_t i = 0; i < length; ++i) {
    v8::Local<v8::Array> _element = v8::Local<v8::Array>::Cast(_points->Get(i));
    points[i][0]                  = _element->Get(0)->NumberValue();
    points[i][1]                  = _element->Get(1)->NumberValue();
  }
  double center[2];
  {
    v8::Local<v8::Array> _centerElement = v8::Local<v8::Array>::Cast(_center);
    center[0]                           = _centerElement->Get(0)->NumberValue();
    center[1]                           = _centerElement->Get(1)->NumberValue();
  }

  // record distances from each location to center
  v8::Local<v8::Array> distances = v8::Array::New(isolate);
  for (size_t i = 0; i < length; ++i) {
    // latitudes
    const double lat1 = radiansFromDeg(center[0]);
    const double lat2 = radiansFromDeg(points[i][0]);

    // dLat, dLng
    const double dlat = radiansFromDeg(points[i][0] - center[0]);
    const double dlng = radiansFromDeg(points[i][1] - center[1]);

    const double distance = haversine(lat1, lat2, dlat, dlng, unit);

    distances->Set(i, v8::Number::New(isolate, distance));
  }

  // create object to hold results
  v8::Local<v8::Object> result = v8::Object::New(isolate);
  result->Set(v8::String::NewFromUtf8(isolate, "origins"), _points);
  result->Set(v8::String::NewFromUtf8(isolate, "destination"), _center);
  result->Set(v8::String::NewFromUtf8(isolate, "distances"), distances);

  args.GetReturnValue().Set(result);
}

void init(v8::Local<v8::Object> exports)
{
  NODE_SET_METHOD(exports, "distance", distance);
}

NODE_MODULE(addon, init);

}  // namespace Cartesian
