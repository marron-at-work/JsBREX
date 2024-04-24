#include <napi.h>

#include "../include/brex/regex/brex.h"
#include "../include/brex/regex/brex_parser.h"
#include "../include/brex/regex/brex_compiler.h"

using namespace Napi;

static std::map<std::u8string, brex::UnicodeRegexExecutor*> g_executableRegexMap;

bool processRegex(std::u8string regex, std::string& err) {
  err.clear();
/*
  auto pr = brex::RegexParser::parseUnicodeRegex(ure, true);
  if(!pr.first.has_value() || !pr.second.empty()) {
    err = "Parse errors in regex:\n";
    for(auto iter = pr.second.begin(); iter != pr.second.end(); ++iter) {
      err += "    " + std::string(iter->msg.cbegin(), iter->msg.cend()) + "\n";
    }
  
    return false;
  }

  std::vector<brex::RegexCompileError> compileerror;
  auto executor = brex::RegexCompiler::compileUnicodeRegexToExecutor(pr.first.value(), {}, {}, false, nullptr, nullptr, compileerror);
  if(!compileerror.empty()) {
    err = "Errors compiling regex";

    return false;
  }
*/
  g_executableRegexMap[regex] = executor;
  return true;
}

Napi::Value Method(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 2) {
    Napi::TypeError::New(env, "Expected a Regex and a String").ThrowAsJavaScriptException();
  }

  if (!info[0].IsString() || !info[1].IsString()) {
    Napi::TypeError::New(env, "Expected a Regex and a String").ThrowAsJavaScriptException();
  }

  std::string regex = info[0].As<Napi::String>().Utf8Value();
  std::string str = info[1].As<Napi::String>().Utf8Value();

  if(g_executableRegexMap.find(regex) == g_executableRegexMap.end()) {
    std::string err;
    if(!processRegex(regex, err)) {
      Napi::TypeError::New(env, err).ThrowAsJavaScriptException();
    }
  }

  return Napi::Boolean::From<bool>(env, regex == str);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "Jsbrex"),
              Napi::Function::New(env, Method));
  return exports;
}

NODE_API_MODULE(addon, Init)
