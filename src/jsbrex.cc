#include <napi.h>

#include "../include/brex/regex/brex.h"
#include "../include/brex/regex/brex_parser.h"
#include "../include/brex/regex/brex_compiler.h"

using namespace Napi;

static std::map<std::u8string, brex::UnicodeRegexExecutor*> g_executableRegexMap;
static std::u8string g_lexstring;

bool processRegex(std::u8string regex, std::string& err) {
  err.clear();

  auto pr = brex::RegexParser::parseUnicodeRegex(regex, true);
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

  g_executableRegexMap[regex] = executor;
  return true;
}

Napi::Value Accepts(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 2) {
    Napi::TypeError::New(env, "Expected a Regex and a String").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsString() || !info[1].IsString()) {
    Napi::TypeError::New(env, "Expected a Regex and a String").ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string uregex = info[0].As<Napi::String>().Utf8Value();
  std::u8string regex(uregex.cbegin(), uregex.cend());

  if(g_executableRegexMap.find(regex) == g_executableRegexMap.end()) {
    std::string err;
    if(!processRegex(regex, err)) {
      Napi::TypeError::New(env, err).ThrowAsJavaScriptException();
      return env.Null();
    }
  }

  std::string ustr = info[1].As<Napi::String>().Utf8Value();
  std::u8string str(ustr.cbegin(), ustr.cend());

  auto executor = g_executableRegexMap[regex];

  brex::ExecutorError err;
  auto accepts = executor->test(&str, err);

  if(err != brex::ExecutorError::Ok) {
    Napi::TypeError::New(env, "Invalid regex form for operation").ThrowAsJavaScriptException();
    return env.Null();
  }

  return Napi::Boolean::From<bool>(env, accepts);
}

Napi::Value StartsWith(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 2) {
    Napi::TypeError::New(env, "Expected a Regex and a String").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsString() || !info[1].IsString()) {
    Napi::TypeError::New(env, "Expected a Regex and a String").ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string uregex = info[0].As<Napi::String>().Utf8Value();
  std::u8string regex(uregex.cbegin(), uregex.cend());

  if(g_executableRegexMap.find(regex) == g_executableRegexMap.end()) {
    std::string err;
    if(!processRegex(regex, err)) {
      Napi::TypeError::New(env, err).ThrowAsJavaScriptException();
      return env.Null();
    }
  }

  std::string ustr = info[1].As<Napi::String>().Utf8Value();
  std::u8string str(ustr.cbegin(), ustr.cend());

  auto executor = g_executableRegexMap[regex];

  brex::ExecutorError err;
  auto accepts = executor->testFront(&str, err);

  if(err != brex::ExecutorError::Ok) {
    Napi::TypeError::New(env, "Invalid regex form for operation").ThrowAsJavaScriptException();
    return env.Null();
  }

  return Napi::Boolean::From<bool>(env, accepts);
}

Napi::Value EndsWith(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 2) {
    Napi::TypeError::New(env, "Expected a Regex and a String").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsString() || !info[1].IsString()) {
    Napi::TypeError::New(env, "Expected a Regex and a String").ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string uregex = info[0].As<Napi::String>().Utf8Value();
  std::u8string regex(uregex.cbegin(), uregex.cend());

  if(g_executableRegexMap.find(regex) == g_executableRegexMap.end()) {
    std::string err;
    if(!processRegex(regex, err)) {
      Napi::TypeError::New(env, err).ThrowAsJavaScriptException();
      return env.Null();
    }
  }

  std::string ustr = info[1].As<Napi::String>().Utf8Value();
  std::u8string str(ustr.cbegin(), ustr.cend());

  auto executor = g_executableRegexMap[regex];

  brex::ExecutorError err;
  auto accepts = executor->testBack(&str, err);

  if(err != brex::ExecutorError::Ok) {
    Napi::TypeError::New(env, "Invalid regex form for operation").ThrowAsJavaScriptException();
    return env.Null();
  }

  return Napi::Boolean::From<bool>(env, accepts);
}

Napi::Value InitializeLexer(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 1) {
    Napi::TypeError::New(env, "Expected a String to initialize Lexer state").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Expected a String to initialize Lexer state").ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string ustr = info[0].As<Napi::String>().Utf8Value();
  g_lexstring = { ustr.cbegin(), ustr.cend() };

  return env.Null();
}

Napi::Value LexFront(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 2) {
    Napi::TypeError::New(env, "Expected a regex to match on and position to start from").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Expected a regex to match on").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[1].IsNumber()) {
    Napi::TypeError::New(env, "Expected start position").ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string uregex = info[0].As<Napi::String>().Utf8Value();
  std::u8string regex(uregex.cbegin(), uregex.cend());

  int64_t spos = info[1].As<Napi::Number>().Int32Value();

  if(g_executableRegexMap.find(regex) == g_executableRegexMap.end()) {
    std::string err;
    if(!processRegex(regex, err)) {
      Napi::TypeError::New(env, err).ThrowAsJavaScriptException();
      return env.Null();
    }
  }

  auto executor = g_executableRegexMap[regex];

  brex::ExecutorError err;
  auto match = executor->matchFront(&g_lexstring, spos, (int64_t)g_lexstring.size() - 1, false, false, err);

  if(err != brex::ExecutorError::Ok) {
    Napi::TypeError::New(env, "Invalid regex form for operation").ThrowAsJavaScriptException();
    return env.Null();
  }

  if(!match.has_value()) {
    return env.Null();
  }
  else {
    auto endpos = match.value();
    Napi::String result = Napi::String::New(env, std::string(g_lexstring.cbegin() + spos, g_lexstring.cbegin() + endpos + 1));

    return result;
  }
}


Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "accepts"), Napi::Function::New(env, Accepts));
  exports.Set(Napi::String::New(env, "startsWith"), Napi::Function::New(env, StartsWith));
  exports.Set(Napi::String::New(env, "endsWith"), Napi::Function::New(env, EndsWith));

  exports.Set(Napi::String::New(env, "initializeLexer"), Napi::Function::New(env, InitializeLexer));
  exports.Set(Napi::String::New(env, "lexFront"), Napi::Function::New(env, LexFront));
  return exports;
}

NODE_API_MODULE(addon, Init)
