{
  "targets": [
    {
      "target_name": "cartesian",
      "sources": [ "./src/native/cartesian.cpp",
      "./src/native/wrapper/cartesian.cpp" ],
      "cflags": [ "-std=c++11" ],
      "xcode_settings": {
        "OTHER_CFLAGS": [ "-std=c++11",  "-stdlib=libc++" ],
        "OTHER_LDFLAGS": [ "-stdlib=libc++" ],
        "MACOSX_DEPLOYMENT_TARGET": "10.10"
      }
    },
    {
      "target_name": "center",
      "sources": [ "./src/native/center.cpp",
      "./src/native/wrapper/center.cpp" ],
      "cflags": [ "-std=c++11" ],
      "xcode_settings": {
        "OTHER_CFLAGS": [ "-std=c++11",  "-stdlib=libc++" ],
        "OTHER_LDFLAGS": [ "-stdlib=libc++" ],
        "MACOSX_DEPLOYMENT_TARGET": "10.10"
      }
    },
    {
      "target_name": "tsp",
      "sources": [ "./src/native/center.cpp", "./src/native/tsp.cpp",
       "./src/native/wrapper/tsp.cpp" ],
      "cflags": [ "-std=c++11" ],
      "xcode_settings": {
        "OTHER_CFLAGS": [ "-std=c++11",  "-stdlib=libc++" ],
        "OTHER_LDFLAGS": [ "-stdlib=libc++" ],
        "MACOSX_DEPLOYMENT_TARGET": "10.10"
      }
    },
    {
      "target_name": "polynomial",
      "sources": [ "./src/native/polynomial.cpp",
      "./src/native/wrapper/polynomial.cpp" ],
      "cflags": [ "-std=c++11" ],
      "xcode_settings": {
        "OTHER_CFLAGS": [ "-std=c++11",  "-stdlib=libc++" ],
        "OTHER_LDFLAGS": [ "-stdlib=libc++" ],
        "MACOSX_DEPLOYMENT_TARGET": "10.10"
      }
    }
  ]
}
