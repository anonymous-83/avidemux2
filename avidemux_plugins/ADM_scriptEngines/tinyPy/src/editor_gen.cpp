// Generated by admPyClass.pl do not edit !
// printTiming -> int pyPrintTiming (IEditor int ) 
static tp_obj zzpy_printTiming(TP)
 {
  tp_obj self = tp_getraw(tp);
  IScriptEngine *engine = (IScriptEngine*)tp_get(tp, tp->builtins, tp_string("userdata")).data.val;
  IEditor *editor = engine->editor();
  TinyParams pm(tp);
  void *me = (void *)pm.asThis(&self, ADM_PYID_EDITOR);

  IEditor *p0 = editor;
  int p1 = pm.asInt();
  int r =   pyPrintTiming(p0,p1); 
  return tp_number(r);
}
// hexDumpFrame -> int pyHexDumpFrame (IEditor int ) 
static tp_obj zzpy_hexDumpFrame(TP)
 {
  tp_obj self = tp_getraw(tp);
  IScriptEngine *engine = (IScriptEngine*)tp_get(tp, tp->builtins, tp_string("userdata")).data.val;
  IEditor *editor = engine->editor();
  TinyParams pm(tp);
  void *me = (void *)pm.asThis(&self, ADM_PYID_EDITOR);

  IEditor *p0 = editor;
  int p1 = pm.asInt();
  int r =   pyHexDumpFrame(p0,p1); 
  return tp_number(r);
}
// getVideoDuration -> double editor->getVideoDuration (void ) 
static tp_obj zzpy_getVideoDuration(TP)
 {
  tp_obj self = tp_getraw(tp);
  IScriptEngine *engine = (IScriptEngine*)tp_get(tp, tp->builtins, tp_string("userdata")).data.val;
  IEditor *editor = engine->editor();
  TinyParams pm(tp);
  void *me = (void *)pm.asThis(&self, ADM_PYID_EDITOR);

  double r =   editor->getVideoDuration(); 
  return tp_number(r);
}
// getDts -> double pyGetDts (IEditor int ) 
static tp_obj zzpy_getDts(TP)
 {
  tp_obj self = tp_getraw(tp);
  IScriptEngine *engine = (IScriptEngine*)tp_get(tp, tp->builtins, tp_string("userdata")).data.val;
  IEditor *editor = engine->editor();
  TinyParams pm(tp);
  void *me = (void *)pm.asThis(&self, ADM_PYID_EDITOR);

  IEditor *p0 = editor;
  int p1 = pm.asInt();
  double r =   pyGetDts(p0,p1); 
  return tp_number(r);
}
// dumpSegment -> void editor->dumpSegment (int ) 
static tp_obj zzpy_dumpSegment(TP)
 {
  tp_obj self = tp_getraw(tp);
  IScriptEngine *engine = (IScriptEngine*)tp_get(tp, tp->builtins, tp_string("userdata")).data.val;
  IEditor *editor = engine->editor();
  TinyParams pm(tp);
  void *me = (void *)pm.asThis(&self, ADM_PYID_EDITOR);

  int p0 = pm.asInt();
  editor->dumpSegment(p0); 
 return tp_None;
}
// dumpRefVideo -> int editor->dumpRefVideos (void ) 
static tp_obj zzpy_dumpRefVideo(TP)
 {
  tp_obj self = tp_getraw(tp);
  IScriptEngine *engine = (IScriptEngine*)tp_get(tp, tp->builtins, tp_string("userdata")).data.val;
  IEditor *editor = engine->editor();
  TinyParams pm(tp);
  void *me = (void *)pm.asThis(&self, ADM_PYID_EDITOR);

  int r =   editor->dumpRefVideos(); 
  return tp_number(r);
}
// nbSegments -> int editor->getNbSegment (void ) 
static tp_obj zzpy_nbSegments(TP)
 {
  tp_obj self = tp_getraw(tp);
  IScriptEngine *engine = (IScriptEngine*)tp_get(tp, tp->builtins, tp_string("userdata")).data.val;
  IEditor *editor = engine->editor();
  TinyParams pm(tp);
  void *me = (void *)pm.asThis(&self, ADM_PYID_EDITOR);

  int r =   editor->getNbSegment(); 
  return tp_number(r);
}
// dumpAllSegments -> void editor->dumpSegments (void ) 
static tp_obj zzpy_dumpAllSegments(TP)
 {
  tp_obj self = tp_getraw(tp);
  IScriptEngine *engine = (IScriptEngine*)tp_get(tp, tp->builtins, tp_string("userdata")).data.val;
  IEditor *editor = engine->editor();
  TinyParams pm(tp);
  void *me = (void *)pm.asThis(&self, ADM_PYID_EDITOR);

  editor->dumpSegments(); 
 return tp_None;
}
// getFrameSize -> int editor->getFrameSize (int ) 
static tp_obj zzpy_getFrameSize(TP)
 {
  tp_obj self = tp_getraw(tp);
  IScriptEngine *engine = (IScriptEngine*)tp_get(tp, tp->builtins, tp_string("userdata")).data.val;
  IEditor *editor = engine->editor();
  TinyParams pm(tp);
  void *me = (void *)pm.asThis(&self, ADM_PYID_EDITOR);

  int p0 = pm.asInt();
  int r =   editor->getFrameSize(p0); 
  return tp_number(r);
}
// getPts -> double pyGetPts (IEditor int ) 
static tp_obj zzpy_getPts(TP)
 {
  tp_obj self = tp_getraw(tp);
  IScriptEngine *engine = (IScriptEngine*)tp_get(tp, tp->builtins, tp_string("userdata")).data.val;
  IEditor *editor = engine->editor();
  TinyParams pm(tp);
  void *me = (void *)pm.asThis(&self, ADM_PYID_EDITOR);

  IEditor *p0 = editor;
  int p1 = pm.asInt();
  double r =   pyGetPts(p0,p1); 
  return tp_number(r);
}
tp_obj zzpy__pyEditor_get(tp_vm *vm)
{
  tp_obj self = tp_getraw(vm);
  IScriptEngine *engine = (IScriptEngine*)tp_get(vm, vm->builtins, tp_string("userdata")).data.val;
  IEditor *editor = engine->editor();
  TinyParams pm(vm);
  void *me=(void *)pm.asThis(&self, ADM_PYID_EDITOR);
  char const *key = pm.asString();
  if (!strcmp(key, "printTiming"))
  {
     return tp_method(vm, self, zzpy_printTiming);
  }
  if (!strcmp(key, "hexDumpFrame"))
  {
     return tp_method(vm, self, zzpy_hexDumpFrame);
  }
  if (!strcmp(key, "getVideoDuration"))
  {
     return tp_method(vm, self, zzpy_getVideoDuration);
  }
  if (!strcmp(key, "getDts"))
  {
     return tp_method(vm, self, zzpy_getDts);
  }
  if (!strcmp(key, "dumpSegment"))
  {
     return tp_method(vm, self, zzpy_dumpSegment);
  }
  if (!strcmp(key, "dumpRefVideo"))
  {
     return tp_method(vm, self, zzpy_dumpRefVideo);
  }
  if (!strcmp(key, "nbSegments"))
  {
     return tp_method(vm, self, zzpy_nbSegments);
  }
  if (!strcmp(key, "dumpAllSegments"))
  {
     return tp_method(vm, self, zzpy_dumpAllSegments);
  }
  if (!strcmp(key, "getFrameSize"))
  {
     return tp_method(vm, self, zzpy_getFrameSize);
  }
  if (!strcmp(key, "getPts"))
  {
     return tp_method(vm, self, zzpy_getPts);
  }
  return tp_get(vm, self, tp_string(key));
}
tp_obj zzpy__pyEditor_set(tp_vm *vm)
{
  tp_obj self = tp_getraw(vm);
  IScriptEngine *engine = (IScriptEngine*)tp_get(vm, vm->builtins, tp_string("userdata")).data.val;
  IEditor *editor = engine->editor();
  TinyParams pm(vm);
  void *me = (void *)pm.asThis(&self, ADM_PYID_EDITOR);
  char const *key = pm.asString();
  return tp_None;
}
// Dctor
static void myDtorpyEditor(tp_vm *vm,tp_obj self)
{
}
// Ctor ()
static tp_obj myCtorpyEditor(tp_vm *vm)
{
  tp_obj self = tp_getraw(vm);
  TinyParams pm(vm);
  void *me = NULL;
  tp_obj cdata = tp_data(vm, ADM_PYID_EDITOR, me);
  cdata.data.info->xfree = myDtorpyEditor;
  tp_set(vm, self, tp_string("cdata"), cdata);
  return tp_None;
}
static tp_obj zzpy__pyEditor_help(TP)
{
	PythonEngine *engine = (PythonEngine*)tp_get(tp, tp->builtins, tp_string("userdata")).data.val;

	engine->callEventHandlers(IScriptEngine::Information, NULL, -1, "printTiming(IEditor,int)\n");
	engine->callEventHandlers(IScriptEngine::Information, NULL, -1, "hexDumpFrame(IEditor,int)\n");
	engine->callEventHandlers(IScriptEngine::Information, NULL, -1, "getVideoDuration(void)\n");
	engine->callEventHandlers(IScriptEngine::Information, NULL, -1, "getDts(IEditor,int)\n");
	engine->callEventHandlers(IScriptEngine::Information, NULL, -1, "dumpSegment(int)\n");
	engine->callEventHandlers(IScriptEngine::Information, NULL, -1, "dumpRefVideo(void)\n");
	engine->callEventHandlers(IScriptEngine::Information, NULL, -1, "nbSegments(void)\n");
	engine->callEventHandlers(IScriptEngine::Information, NULL, -1, "dumpAllSegments(void)\n");
	engine->callEventHandlers(IScriptEngine::Information, NULL, -1, "getFrameSize(int)\n");
	engine->callEventHandlers(IScriptEngine::Information, NULL, -1, "getPts(IEditor,int)\n");

	return tp_None;
};
tp_obj initClasspyEditor(tp_vm *vm)
{
  tp_obj myClass = tp_class(vm);
  tp_set(vm,myClass, tp_string("__init__"), tp_fnc(vm,myCtorpyEditor));
  tp_set(vm,myClass, tp_string("__set__"), tp_fnc(vm,zzpy__pyEditor_set));
  tp_set(vm,myClass, tp_string("__get__"), tp_fnc(vm,zzpy__pyEditor_get));
  tp_set(vm,myClass, tp_string("help"), tp_fnc(vm,zzpy__pyEditor_help));
  return myClass;
}
