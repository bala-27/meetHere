{
  "targets": [
    {
      "target_name": "center",
      "sources": [ "./src/native/center.cc" ],
      "cflags": [ "-std=c++11",  "-stdlib=libc++" ],
      "xcode_settings": {
        "OTHER_CFLAGS": [ "-std=c++11",  "-stdlib=libc++" ],
        "OTHER_LDFLAGS": [ "-stdlib=libc++" ],
        "MACOSX_DEPLOYMENT_TARGET": "10.7"
      },
    },
    {
      "target_name": "tsp",
      "sources": [ "./src/native/tsp.cc" ],
      "cflags": [ "-std=c++11",  "-stdlib=libc++" ],
      "xcode_settings": {
        "OTHER_CFLAGS": [ "-std=c++11",  "-stdlib=libc++" ],
        "OTHER_LDFLAGS": [ "-stdlib=libc++" ],
        "MACOSX_DEPLOYMENT_TARGET": "10.7"
      },
    }
  ]
}
