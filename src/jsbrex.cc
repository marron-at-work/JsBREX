#include <napi.h>

#include "../include/brex/regex/brex.h"
#include "../include/brex/regex/brex_parser.h"
#include "../include/brex/regex/brex_compiler.h"
#include "../include/brex/regex/brex_system.h"

using namespace Napi;

static std::map<std::u8string, brex::UnicodeRegexExecutor*> g_executableRegexMap;
static std::u8string g_lexstring;

static std::map<std::string, brex::UnicodeRegexExecutor*> g_systemUnicodeRegexMap;
static std::map<std::string, brex::CRegexExecutor*> g_systemCStringRegexMap;

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
    return env.Undefined();
  }

  if (!info[0].IsString() || !info[1].IsString()) {
    return env.Undefined();
  }

  std::string uregex = info[0].As<Napi::String>().Utf8Value();
  std::u8string regex(uregex.cbegin(), uregex.cend());

  if(g_executableRegexMap.find(regex) == g_executableRegexMap.end()) {
    std::string err;
    if(!processRegex(regex, err)) {
      return env.Undefined();
    }
  }

  std::string ustr = info[1].As<Napi::String>().Utf8Value();
  std::u8string str(ustr.cbegin(), ustr.cend());

  auto executor = g_executableRegexMap[regex];

  brex::ExecutorError err;
  auto accepts = executor->test(&str, err);

  if(err != brex::ExecutorError::Ok) {
    return env.Undefined();
  }

  return Napi::Boolean::From<bool>(env, accepts);
}

Napi::Value StartsWith(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 2) {
    return env.Undefined();
  }

  if (!info[0].IsString() || !info[1].IsString()) {
    return env.Undefined();
  }

  std::string uregex = info[0].As<Napi::String>().Utf8Value();
  std::u8string regex(uregex.cbegin(), uregex.cend());

  if(g_executableRegexMap.find(regex) == g_executableRegexMap.end()) {
    std::string err;
    if(!processRegex(regex, err)) {
      return env.Undefined();
    }
  }

  std::string ustr = info[1].As<Napi::String>().Utf8Value();
  std::u8string str(ustr.cbegin(), ustr.cend());

  auto executor = g_executableRegexMap[regex];

  brex::ExecutorError err;
  auto accepts = executor->testFront(&str, err);

  if(err != brex::ExecutorError::Ok) {
    return env.Undefined();
  }

  return Napi::Boolean::From<bool>(env, accepts);
}

Napi::Value EndsWith(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 2) {
    return env.Undefined();
  }

  if (!info[0].IsString() || !info[1].IsString()) {
    return env.Undefined();
  }

  std::string uregex = info[0].As<Napi::String>().Utf8Value();
  std::u8string regex(uregex.cbegin(), uregex.cend());

  if(g_executableRegexMap.find(regex) == g_executableRegexMap.end()) {
    std::string err;
    if(!processRegex(regex, err)) {
      return env.Undefined();
    }
  }

  std::string ustr = info[1].As<Napi::String>().Utf8Value();
  std::u8string str(ustr.cbegin(), ustr.cend());

  auto executor = g_executableRegexMap[regex];

  brex::ExecutorError err;
  auto accepts = executor->testBack(&str, err);

  if(err != brex::ExecutorError::Ok) {
    return env.Undefined();
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
      Napi::Error::New(env, err).ThrowAsJavaScriptException();
      return env.Null();
    }
  }

  auto executor = g_executableRegexMap[regex];

  brex::ExecutorError err;
  auto match = executor->matchFront(&g_lexstring, spos, (int64_t)g_lexstring.size() - 1, err);

  if(err != brex::ExecutorError::Ok) {
    Napi::Error::New(env, "Invalid regex form for operation").ThrowAsJavaScriptException();
    return env.Null();
  }

  if(!match.has_value()) {
    return env.Null();
  }
  else {
    Napi::String result = Napi::String::New(env, std::string(g_lexstring.cbegin() + spos, g_lexstring.cbegin() + match.value() + 1));

    return result;
  }
}

Napi::Value LoadConstAndValidateRESystem(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 1 || !info[0].IsArray()) {
    Napi::TypeError::New(env, "Expected a regex system (array of nsinfos)").ThrowAsJavaScriptException();
    return env.Null();
  }

  Napi::Array systemarray = info[0].As<Napi::Array>();
  std::vector<brex::RENSInfo> ninfos;
  for(size_t i = 0; i < systemarray.Length(); ++i) {
    if(!systemarray.Get(i).IsObject()) {
      Napi::TypeError::New(env, "Expected a regex system namepsace entry").ThrowAsJavaScriptException();
      return env.Null();
    }

    Napi::Object ninfoentry = systemarray.Get(i).As<Napi::Object>();
    if(!ninfoentry.Has("nsinfo") || !ninfoentry.Get("nsinfo").IsObject() || !ninfoentry.Has("reinfos") || !ninfoentry.Get("reinfos").IsArray()) {
      Napi::TypeError::New(env, "Expected fields nsinfo (object) and reinfos (array)").ThrowAsJavaScriptException();
      return env.Null();
    }

    Napi::Object remapinfo = ninfoentry.Get("nsinfo").As<Napi::Object>();
    if(!remapinfo.Has("inns") || !remapinfo.Get("inns").IsString() || !remapinfo.Has("nsmappings") || !remapinfo.Get("nsmappings").IsArray()) {
      Napi::TypeError::New(env, "Expected fields inns (string) and nsmappings (array)").ThrowAsJavaScriptException();
      return env.Null();
    }

    std::vector<std::pair<std::string, std::string>> nsmappings;
    Napi::Array mappingarr = remapinfo.Get("nsmappings").As<Napi::Array>();
    size_t mappinglen = mappingarr.Length();
    for(size_t j = 0; j < mappinglen; ++j) {
      Napi::Value nsmappingentry = mappingarr.Get(j);
      if(!nsmappingentry.IsArray()){
        Napi::TypeError::New(env, "Expected namespace remapping pair").ThrowAsJavaScriptException();
        return env.Null();
      }

      Napi::Array nsmappingarr = nsmappingentry.As<Napi::Array>();
      if(nsmappingarr.Length() != 2 || !nsmappingarr.Get((uint32_t)0).IsString() || !nsmappingarr.Get((uint32_t)1).IsString()) {
        Napi::TypeError::New(env, "Expected namespace remapping pair").ThrowAsJavaScriptException();
        return env.Null();
      }

      nsmappings.push_back({nsmappingarr.Get((uint32_t)0).As<Napi::String>().Utf8Value(), nsmappingarr.Get((uint32_t)1).As<Napi::String>().Utf8Value()});
    }

    Napi::Array reinfos = ninfoentry.Get("reinfos").As<Napi::Array>();
    size_t reinfoslen = reinfos.Length();
    std::vector<brex::REInfo> reinfovec;
    for(size_t j = 0; j < reinfoslen; ++j) {
      if(!reinfos.Get(j).IsObject()) {
        Napi::TypeError::New(env, "Expected a regex entry").ThrowAsJavaScriptException();
        return env.Null();
      }

      Napi::Object reinfoentry = reinfos.Get(j).As<Napi::Object>();
      if(!reinfoentry.Has("name") || !reinfoentry.Get("name").IsString() || !reinfoentry.Has("restr") || !reinfoentry.Get("restr").IsString()) {
        Napi::TypeError::New(env, "Expected fields name (string) and regex (string)").ThrowAsJavaScriptException();
        return env.Null();
      }

      std::string rename = reinfoentry.Get("name").As<Napi::String>().Utf8Value();
      std::string restring = reinfoentry.Get("restr").As<Napi::String>().Utf8Value();

      std::u8string u8restring = std::u8string(restring.cbegin(), restring.cend());

      reinfovec.push_back({rename, u8restring});
    }

    ninfos.push_back({{remapinfo.Get("inns").As<Napi::String>().Utf8Value(), nsmappings}, reinfovec});
  }

  std::vector<std::u8string> errors;
  brex::ReSystem rsystem = brex::ReSystem::processSystem(ninfos, errors);

  if(!errors.empty()) {
    auto arr = Napi::Array::New(env);
    for(size_t i = 0; i < errors.size(); ++i) {
      arr.Set(i, Napi::String::New(env, std::string(errors[i].cbegin(), errors[i].cend())));
    }

    return arr;
  }

  for(size_t i = 0; i < rsystem.entries.size(); ++i) {
    if(rsystem.entries[i]->isUnicode()) {
      g_systemUnicodeRegexMap[rsystem.entries[i]->fullname] = static_cast<brex::ReSystemUnicodeEntry*>(rsystem.entries[i])->executor;
    }
    else {
      g_systemCStringRegexMap[rsystem.entries[i]->fullname] = static_cast<brex::ReSystemCEntry*>(rsystem.entries[i])->executor;
    }
  }

  return env.Null();
}

Napi::Value RunNamedRegexAccepts(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 3) {
    Napi::TypeError::New(env, "Expected a regex name, string to validate, and a boolean indicating if the regex is unicode").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!info[0].IsString() || !info[1].IsString() || !info[2].IsBoolean()) {
    Napi::TypeError::New(env, "Expected a regex name, string to validate, and a boolean indicating if the regex is unicode").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  std::string rname = info[0].As<Napi::String>().Utf8Value();
  std::string ustr = info[1].As<Napi::String>().Utf8Value();

  if(info[2].As<Napi::Boolean>().Value()) {
    if(g_systemUnicodeRegexMap.find(rname) == g_systemUnicodeRegexMap.end()) {
      Napi::TypeError::New(env, "Named regex not found " + rname).ThrowAsJavaScriptException();
      return env.Undefined();
    }

    auto executor = g_systemUnicodeRegexMap[rname];
    std::u8string str(ustr.cbegin(), ustr.cend());

    brex::ExecutorError err;
    auto accepts = executor->test(&str, err);

    if(err != brex::ExecutorError::Ok) {
      Napi::Error::New(env, "Error validating regex").ThrowAsJavaScriptException();
      return env.Undefined();
    }

    return Napi::Boolean::From<bool>(env, accepts);
  }
  else {
    if(g_systemCStringRegexMap.find(rname) == g_systemCStringRegexMap.end()) {
      Napi::TypeError::New(env, "Named regex not found " + rname).ThrowAsJavaScriptException();
      return env.Undefined();
    }

    auto executor = g_systemCStringRegexMap[rname];
    std::string str(ustr.cbegin(), ustr.cend());

    brex::ExecutorError err;
    auto accepts = executor->test(&str, err);

    if(err != brex::ExecutorError::Ok) {
      Napi::Error::New(env, "Error validating regex").ThrowAsJavaScriptException();
      return env.Undefined();
    }

    return Napi::Boolean::From<bool>(env, accepts);
  }
}

Napi::Value ValidateStringLiteral(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 1) {
    Napi::TypeError::New(env, "Expected a String to validate").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Expected a String to validate").ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string ustr = info[0].As<Napi::String>().Utf8Value();
  std::u8string str(ustr.cbegin(), ustr.cend());

  std::pair<std::optional<brex::UnicodeString>, std::optional<std::u8string>> uestr = brex::unescapeUnicodeStringLiteralInclMultiline((uint8_t*)str.c_str(), str.size());
  if(!uestr.first.has_value()) {
    Napi::Error::New(env, std::string(uestr.second.value().cbegin(), uestr.second.value().cend())).ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string rstr = std::string(uestr.first.value().cbegin(), uestr.first.value().cend());
  return Napi::String::From<std::string>(env, rstr);
}

Napi::Value ValidateCStringLiteral(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 1) {
    Napi::TypeError::New(env, "Expected a String to validate").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Expected a String to validate").ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string ustr = info[0].As<Napi::String>().Utf8Value();
  std::u8string str(ustr.cbegin(), ustr.cend());

  std::pair<std::optional<brex::CString>, std::optional<std::u8string>> uestr = brex::unescapeCStringLiteralInclMultiline((uint8_t*)str.c_str(), str.size());
  if(!uestr.first.has_value()) {
    Napi::Error::New(env, std::string(uestr.second.value().cbegin(), uestr.second.value().cend())).ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string rstr = std::string(uestr.first.value().cbegin(), uestr.first.value().cend());
  return Napi::String::From<std::string>(env, rstr);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "accepts"), Napi::Function::New(env, Accepts));
  exports.Set(Napi::String::New(env, "startsWith"), Napi::Function::New(env, StartsWith));
  exports.Set(Napi::String::New(env, "endsWith"), Napi::Function::New(env, EndsWith));

  exports.Set(Napi::String::New(env, "initializeLexer"), Napi::Function::New(env, InitializeLexer));
  exports.Set(Napi::String::New(env, "lexFront"), Napi::Function::New(env, LexFront));

  exports.Set(Napi::String::New(env, "validateStringLiteral"), Napi::Function::New(env, ValidateStringLiteral));
  exports.Set(Napi::String::New(env, "validateCStringLiteral"), Napi::Function::New(env, ValidateCStringLiteral));

  exports.Set(Napi::String::New(env, "loadConstAndValidateRESystem"), Napi::Function::New(env, LoadConstAndValidateRESystem));
  exports.Set(Napi::String::New(env, "runNamedRegexAccepts"), Napi::Function::New(env, RunNamedRegexAccepts));

  return exports;
}

NODE_API_MODULE(addon, Init)
