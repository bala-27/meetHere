{
  "targets": [
    {
      "target_name": "cartesian",
      "sources": [ "./src/native/cartesian.cc" ],
      "cflags": [ "-std=c++11" ],
      "xcode_settings": {
        "OTHER_CFLAGS": [ "-std=c++11",  "-stdlib=libc++" ],
        "OTHER_LDFLAGS": [ "-stdlib=libc++" ],
        "MACOSX_DEPLOYMENT_TARGET": "10.7"
      }
    },
    {
      "target_name": "center",
      "sources": [ "./src/native/center.cc" ],
      "cflags": [ "-std=c++11" ],
      "xcode_settings": {
        "OTHER_CFLAGS": [ "-std=c++11",  "-stdlib=libc++" ],
        "OTHER_LDFLAGS": [ "-stdlib=libc++" ],
        "MACOSX_DEPLOYMENT_TARGET": "10.7"
      }
    },
    {
      "target_name": "tsp",
      "sources": [ "./src/native/tsp.cc" ],
      "cflags": [ "-std=c++11" ],
      "xcode_settings": {
        "OTHER_CFLAGS": [ "-std=c++11",  "-stdlib=libc++" ],
        "OTHER_LDFLAGS": [ "-stdlib=libc++" ],
        "MACOSX_DEPLOYMENT_TARGET": "10.7"
      }
    },
    {
      "target_name": "polynomial",
      "sources": [ "./src/native/polynomial.cc" ],
      "cflags": [ "-std=c++11" ],
      "xcode_settings": {
        "OTHER_CFLAGS": [ "-std=c++11",  "-stdlib=libc++" ],
        "OTHER_LDFLAGS": [ "-stdlib=libc++" ],
        "MACOSX_DEPLOYMENT_TARGET": "10.7"
      }
    }
  ]
}
