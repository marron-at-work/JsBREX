{
  'targets': [
    {
      'target_name': 'jsbrex-native',
      'sources': [ 'src/jsbrex.cc' ],
      'include_dirs': [
          "<!@(node -p \"require('node-addon-api').include\")", 
          "include/json/"
      ],
      'libraries': [ 
          "-Linclude/brex/"
      ],
      'dependencies': ["<!(node -p \"require('node-addon-api').gyp\")"],
      'cflags!': [ '-fno-exceptions', '-std=gnu++20' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'cflags_cc': [ '-std=gnu++20' ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ]
    }
  ]
}