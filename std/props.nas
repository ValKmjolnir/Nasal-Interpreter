##
# Node class definition.  The class methods simply wrap the
# low level extension functions which work on a "ghost" handle to a
# SGPropertyNode object stored in the _g field.
#
# Not all of the features of SGPropertyNode are supported.  There is
# no support for ties, obviously, as that wouldn't make much sense
# from a Nasal context.  The various get/set methods work only on the
# local node, there is no equivalent of the "relative path" variants
# available in C++; just use node.getNode(path).whatever() instead.
#
use std.math;
use std.string;

var _new = func {
    return {
        type: "",
        name: "",
        value: nil,
        parent_list:[],
        child_list:[]
    };
}

var _fg_props_globals = _new();
var _globals = func {
    return _fg_props_globals;
}

var getprop = func(arg...) {
    die("unimplemented");
}

var addcommand = func(name, code) {
    die("unimplemented");
}

var fgcommand = func(name, args = nil) {
    die("unimplemented");
}

var _createCondition = func {
    die("unimplemented");
}

var _getNode = func {
    die("unimplemented");
};

var _getParent = func {
    die("unimplemented");
};

var _getChildren = func {
    die("unimplemented");
};

var _setChildren = func {
    die("unimplemented");
};

var _alias = func {
    die("unimplemented");
};

var _equals = func {
    die("unimplemented");
};

var _unalias = func {
    die("unimplemented");
};

var _adjustValue = func {
    die("unimplemented");
};

var _setDoubleValue = func {
    die("unimplemented");
};

var _setIntValue = func {
    die("unimplemented");
};

var _setBoolValue = func {
    die("unimplemented");
};

var _toggleBoolValue = func {
    die("unimplemented");
};

var _setValue = func {
    die("unimplemented");
};

var _setValues = func {
    die("unimplemented");
};

var _getAttribute = func {
    die("unimplemented");
};

var _setAttribute = func {
    die("unimplemented");
};

var _getValue = func {
    die("unimplemented");
};

var _getType = func {
    die("unimplemented");
};

var _isNumeric = func {
    die("unimplemented");
};

var _isInt = func {
    die("unimplemented");
};

var _getIndex = func {
    die("unimplemented");
};

var _getName = func {
    die("unimplemented");
};

var _getAliasTarget = func {
    die("unimplemented");
};

var _getChild = func {
    die("unimplemented");
};

var _addChild = func {
    die("unimplemented");
};

var _addChildren = func {
    die("unimplemented");
};

var _removeChild = func {
    die("unimplemented");
};

var _removeChildren = func {
    die("unimplemented");
};

var _removeAllChildren = func {
    die("unimplemented");
};


var Node = {
    getNode          : func wrap(_getNode(me._g, arg)),
    getParent        : func wrap(_getParent(me._g, arg)),
    getChild         : func wrap(_getChild(me._g, arg)),
    getChildren      : func wrap(_getChildren(me._g, arg)),
    setChildren      : func wrap(_setChildren(me._g, arg)),
    addChild         : func wrap(_addChild(me._g, arg)),
    addChildren      : func wrap(_addChildren(me._g, arg)),
    removeChild      : func wrap(_removeChild(me._g, arg)),
    removeChildren   : func wrap(_removeChildren(me._g, arg)),
    removeAllChildren: func wrap(_removeAllChildren(me._g, arg)),
    getAliasTarget   : func wrap(_getAliasTarget(me._g, arg)),

    getName        : func _getName(me._g, arg),
    getIndex       : func _getIndex(me._g, arg),
    getType        : func _getType(me._g, arg),
    isNumeric      : func _isNumeric(me._g, arg),
    isInt          : func _isInt(me._g, arg),
    getAttribute   : func _getAttribute(me._g, arg),
    setAttribute   : func _setAttribute(me._g, arg),
    getValue       : func _getValue(me._g, arg),
    setValue       : func _setValue(me._g, arg),
    setValues      : func _setValues(me._g, arg),
    setIntValue    : func _setIntValue(me._g, arg),
    setBoolValue   : func _setBoolValue(me._g, arg),
    toggleBoolValue: func _toggleBoolValue(me._g, arg),
    setDoubleValue : func _setDoubleValue(me._g, arg),
    # adjustValue works like setDoubleValue but adds the argument to the 
    # current value of the node
    adjustValue    : func _adjustValue(me._g, arg),
    unalias        : func _unalias(me._g, arg),
    alias          : func(n) _alias(me._g, [isa(n, Node) ? n._g : n]),
    equals         : func(n) _equals(me._g, [isa(n, Node) ? n._g : n]),
    clearValue     : func _alias(me._g, [_globals()]) and me.unalias(),

    getPath : func {
        var (name, index, parent) = (me.getName(), me.getIndex(), me.getParent());
        if (index != 0)    { name ~= "[" ~ index ~ "]"; }
        if (parent != nil) { name = parent.getPath() ~ "/" ~ name; }
        return name;
    },

    getBoolValue : func {
        var val = me.getValue();
        var mytype = me.getType();
        if ((mytype == "STRING" or mytype == "UNSPECIFIED") and val == "false") return 0;
        return !!val;
    },

    getIntValue : func {
        return math.round(me.getValue());
    },

    getDoubleValue : func {
        return num(me.getValue());
    },

    remove : func {
        if ((var p = me.getParent()) == nil) return nil;
        p.removeChild(me.getName(), me.getIndex());
    },

    # follow alias links to "real" node (e.g. where the data is stored)
    # optional argument: property node or prop path to resolve
    # if no argument is given, operate on current obj ("me")
    resolveAlias : func(p = nil) {
        if (p == nil)
            p = me;
        elsif (isscalar(p))
            p = globals.getNode(p);
        if (isa(p, Node)) {
            while (p.getAttribute("alias")) {
                p = p.getAliasTarget();
            }
        }
        return p;
    },
    
    # add n to int property, do nothing, if prop type is not int
    # n will be integer and defaults to 1
    increment: func(n = 1) {
        if (me.isInt()) {
            var v = me.getValue() + int(n);
            if (me.setIntValue(v)) return v;
        }
        else  {
            logprint(DEV_ALERT, "props.increment() can be used only on integer props "~
                me.getPath()~" "~me.getType());
            return nil;
        }
    },
    
    # sub n from int property, do nothing, if prop type is not int
    # n will be integer and defaults to 1
    decrement: func(n = 1) {
        if (me.isInt())
            var v = me.getValue() - int(n);
            if (me.setIntValue(v)) return v;
        else {
            logprint(DEV_ALERT, "props.decrement() can be used only on integer props"~
                me.getPath()~" "~me.getType());
            return 0;
        }
    },
    
    # checks if a string can be used as a prop name
    # returns 0 (invalid) or 1 (valid)
    # as string operations are expensive, use this only when necessary, 
    # especially do not use in update loops
    isValidPropName: func(s) {
        if (!size(s)) return 0;
        var _ispropspecial =  func(c) { c == `_` or c == `.` or c == `-`; }
        var _ispropname =  func(c) string.isalnum(c) or _ispropspecial(c);
        if (!string.isalpha(s[0]) and s[0] != `_`) return 0;
        for (var i=1; i < size(s); i += 1) {
            if (!_ispropname(s[i])) return 0;
        }
        return 1;
    },

    # replaces any invalid char by "_"
    # as string operations are expensive, use this only when necessary, 
    # especially do not use in update loops
    makeValidPropName: func(s) {
        if (!size(s)) return nil;
        var _ispropspecial =  func(c) { c == `_` or c == `.` or c == `-`; }
        var _ispropname =  func(c) string.isalnum(c) or _ispropspecial(c);
        var rv = "";
        rv ~= (!string.isalpha(s[0])) ?  "_" : chr(s[0]);
        for (var i=1; i < size(s); i += 1) {
            rv ~= (_ispropname(s[i])) ? chr(s[i]) : "_";
        }
        return rv;
    },    
};

##
# Static constructor for a Node object.  Accepts a Nasal hash
# expression to initialize the object a-la setValues().
#
Node.new = func(values = nil) {
    var result = wrapNode(_new());
    if (ishash(values)) {
        result.setValues(values);
    }
    return result;
}

##
# Counter piece of setValues(). Returns a hash with all values
# in the subtree. Nodes with same name are returned as vector,
# where the original node indices are lost. The function should
# only be used if all or almost all values are needed, and never
# in performance-critical code paths. If it's called on a node
# without children, then the result is equivalent to getValue().
#
Node.getValues = func {
    var children = me.getChildren();
    if (!size(children)) return me.getValue();
    var val = {};
    var numchld = {};
    foreach(var c; children) {
        var name = c.getName();
        if (contains(numchld, name)) { var nc = numchld[name]; }
        else {
            var nc = size(me.getChildren(name));
            numchld[name] = nc;
            if (nc > 1 and !contains(val, name)) val[name] = [];
        }
        if (nc > 1) append(val[name], c.getValues());
        else val[name] = c.getValues();
    }
    return val;
}

##
# Initializes property if it's still undefined.  First argument
# is a property name/path. It can also be nil or an empty string,
# in which case the node itself gets initialized, rather than one
# of its children.  Second argument is the default value. The third,
# optional argument is a property type (one of "STRING", "DOUBLE",
# "INT", or "BOOL").  If it is omitted, then "DOUBLE" is used for
# numbers, and STRING for everything else.  Returns the property
# as props.Node.  The fourth optional argument enforces a type if
# non-zero.
#
Node.initNode = func(path = nil, value = 0, type = nil, force = 0) {
    var prop = me.getNode(path or "", 1);
    if (prop.getType() != "NONE") value = prop.getValue();
    if (force) prop.clearValue();
    if (type == nil) prop.setValue(value);
    elsif (type == "DOUBLE") prop.setDoubleValue(value);
    elsif (type == "INT") prop.setIntValue(value);
    elsif (type == "BOOL") prop.setBoolValue(value);
    elsif (type == "STRING") prop.setValue("" ~ value);
    else die("initNode(): unsupported type '" ~ type ~ "'");
    return prop;
}

##
# Useful debugging utility.  Recursively dumps the full state of a
# Node object to the console.  Try binding "props.dump(props.globals)"
# to a key for a fun hack.
#
var dump = func {
    if (size(arg) == 1) { var prefix = "";     var node = arg[0]; }
    else               { var prefix = arg[0]; var node = arg[1]; }

    var index = node.getIndex();
    var type = node.getType();
    var name = node.getName();
    var val = node.getValue();

    if (val == nil) { val = "nil"; }
    name = prefix ~ name;
    if (index > 0) { name = name ~ "[" ~ index ~ "]"; }
    print(name, " {", type, "} = ", val);

    # Don't recurse into aliases, lest we get stuck in a loop
    if (type != "ALIAS") {
        var children = node.getChildren();
        foreach(var c; children) { dump(name ~ "/", c); }
    }
}

##
# Recursively copy property branch from source Node to
# destination Node. Doesn't copy aliases. Copies attributes
# if optional third argument is set and non-zero.
#
var copy = func(src, dest, attr = 0) {
    var sp = src.getPath() or "";
    var dp = dest.getPath() or "";
    
    # sp and dp may be equal but on different trees! 
    # check if dest is sub node of source
    sp = split("/", sp);
    dp = split("/", dp);
    while (size(sp) and size(dp) and (sp[0] == dp[0])) {
        sp = subvec(sp, 1);
        dp = subvec(dp, 1);
    }
    
    foreach(var c; src.getChildren()) {
        var name = c.getName();
        var i = c.getIndex();
        if (i) name ~= "["~i~"]";
        if (!(!size(sp) and size(dp) and name == dp[0]))
            copy(src.getNode(name), dest.getNode(name, 1), attr);
        else {
            logprint(DEV_WARN, "props.copy() skipping "~name~" (recursion!)");
        }
    }
    var type = src.getType();
    var val = src.getValue();
    if (type == "ALIAS" or type == "NONE") return;
    elsif (type == "BOOL") dest.setBoolValue(val);
    elsif (type == "INT" or type == "LONG") dest.setIntValue(val);
    elsif (type == "FLOAT" or type == "DOUBLE") dest.setDoubleValue(val);
    else dest.setValue(val);
    if (attr) dest.setAttribute(src.getAttribute());
}

##
# Utility.  Turns any ghosts it finds (either solo, or in an
# array) into Node objects.
#
var wrap = func(node) {
    if (isghost(node)) {
        return wrapNode(node);
    } elsif (isvec(node)) {
        var v = node;
        var n = size(v);
        for(var i=0; i<n; i+=1) { v[i] = wrapNode(v[i]); }
        return v;
    }
    return node;
}

##
# Utility.  Returns a new object with its superclass/parent set to the
# Node object and its _g (ghost) field set to the specified object.
# Nasal's literal syntax can be pleasingly terse. I like that. :)
#
var wrapNode = func(node) {
    return { parents : [Node], _g : node };
}

##
# Global property tree.  Set once at initialization.  Is that OK?
# Does anything ever call globals.set_props() from C++?  May need to
# turn this into a function if so.
#
var globals = func {
    return wrapNode(_globals());
}();

##
# Shortcut for props.globals.getNode().
#
var getNode = func return call(props.globals.getNode, arg, props.globals);

##
# Sets all indexed property children to a single value.  arg[0]
# specifies a property name (e.g. /controls/engines/engine), arg[1] a
# path under each node of that name to set (e.g. "throttle"), arg[2]
# is the value.
#
var setAll = func(base, child, value) {
    var node = props.globals.getNode(base);
    if (node == nil) return;
    var name = node.getName();
    node = node.getParent();
    if (node == nil) return;
    var children = node.getChildren();
    foreach(var c; children)
        if (c.getName() == name)
            c.getNode(child, 1).setValue(value);
}

# createNodeObjectsFromHash - create nasal node objects from hash
# property_list: hash; where keys are variable names and values are
#                property paths
#                { foo: "/some/prop/foo", bar: "/some/other/prop", }
# namespace:     optional; variables (objects) are created in this namespace
#                defaults to namespace of caller, e.g. after calling this
#                you can use foo.getValue() or bar.addChild()
#
var createNodeObjectsFromHash = func (property_list, namespace = nil) {
    if (namespace == nil) {
        namespace = caller(1)[0];
    }
    if (!ishash(namespace)) {
        logprint(LOG_WARN, "createNodeObjectsFromHash: Error, namespace argument is not a hash.");
        return nil;
    }
    if (!ishash(property_list)) {
        logprint(LOG_WARN, "createNodeObjectsFromHash: Error, property_list argument is not a hash.");
        return nil;
    }
    foreach (var key; keys(property_list)) {
        namespace[key] = props.getNode(property_list[key],1);
    }
}

##
# Turns about anything into a list of props.Nodes, including ghosts,
# path strings, vectors or hashes containing, as well as functions
# returning any of the former and in arbitrary nesting. This is meant
# to be used in functions whose main purpose is to handle collections
# of properties.
#
var nodeList = func {
    var list = [];
    foreach(var a; arg) {
        if (isa(a, Node))
            append(list, a);
        elsif (isscalar(a))
            append(list, props.globals.getNode(a, 1));
        elsif (isvec(a))
            foreach(var i; a)
                list ~= nodeList(i);
        elsif (ishash(a))
            foreach(var i; keys(a))
                list ~= nodeList(a[i]);
        elsif (isfunc(a))
            list ~= nodeList(a());
        elsif (isghost(a) and ghosttype(a) == "prop")
            append(list, wrapNode(a));
        else
            die("nodeList: invalid nil property");
    }
    return list;
}

##
# Compiles a <condition> property branch according to the rules
# set out in $FG_ROOT/Docs/README.conditions into a Condition object.
# The 'test' method of the returend object can be used to evaluate
# the condition.
# The function returns nil on error.
#
var compileCondition = func(p) {
    if (p == nil) return nil;
    if (!isa(p, Node)) p = props.globals.getNode(p);
    return _createCondition(p._g);
}

##
# Evaluates a <condition> property branch according to the rules
# set out in $FG_ROOT/Docs/README.conditions. Undefined conditions
# and a nil argument are "true". The function dumps the condition
# branch and returns nil on error.
#
var condition = func(p) {
    if (p == nil) return 1;
    if (!isa(p, Node)) p = props.globals.getNode(p);
    return _cond_and(p)
}

var _cond_and = func(p) {
    foreach(var c; p.getChildren())
        if (!_cond(c)) return 0;
    return 1;
}

var _cond_or = func(p) {
    foreach(var c; p.getChildren())
        if (_cond(c)) return 1;
    return 0;
}

var _cond = func(p) {
    var n = p.getName();
    if (n == "or") return _cond_or(p);
    if (n == "and") return _cond_and(p);
    if (n == "not") return !_cond_and(p);
    if (n == "equals") return _cond_cmp(p, 0);
    if (n == "not-equals") return !_cond_cmp(p, 0);
    if (n == "less-than") return _cond_cmp(p, -1);
    if (n == "greater-than") return _cond_cmp(p, 1);
    if (n == "less-than-equals") return !_cond_cmp(p, 1);
    if (n == "greater-than-equals") return !_cond_cmp(p, -1);
    if (n == "property") return !!getprop(p.getValue());
    logprint(LOG_ALERT, "condition: invalid operator ", n);
    dump(p);
    return nil;
}

var _cond_cmp = func(p, op) {
    var left = p.getChild("property", 0, 0);
    if (left != nil) { left = getprop(left.getValue()); }
    else {
        logprint(LOG_ALERT, "condition: no left value");
        dump(p);
        return nil;
    }
    var right = p.getChild("property", 1, 0);
    if (right != nil) { right = getprop(right.getValue()); }
    else {
        right = p.getChild("value", 0, 0);
        if (right != nil) { right = right.getValue(); }
        else {
            logprint(LOG_ALERT, "condition: no right value");
            dump(p);
            return nil;
        }
    }

    if (left == nil) left = 0.0;
    if (right == nil) right = 0.0;

    if (op < 0) return left < right;
    if (op > 0) return left > right;
    return left == right;
}

##
# Runs <binding> as described in $FG_ROOT/Docs/README.commands using
# a given module by default, and returns 1 if fgcommand() succeeded,
# or 0 otherwise. The module name won't override a <module> defined
# in the binding.
#
var runBinding = func(node, module = nil) {
    if (module != nil and node.getNode("module") == nil)
        node.getNode("module", 1).setValue(module);
    var cmd = node.getNode("command", 1).getValue() or "null";
    condition(node.getNode("condition")) ? fgcommand(cmd, node) : 0;
}

 #---------------------------------------------------------------------------
 # Property / object update manager
 #
 # - Manage updates when a value has changed more than a predetermined amount.
 #   This class is designed to make updating displays (e.g. canvas), or
 #   performing actions based on a property (or value in a hash) changing
 #   by more than the preset amount.
 #   This can make a significant improvement to performance compared to simply
 #   redrawing a canvas in an update loop.
 # - Author       : Richard Harrison (rjh@zaretto.com)
 #---------------------------------------------------------------------------*/

#example usage:
# this is using the hashlist (which works well with an Emesary notification)
# basically when the method is called it will call each section (in the lambda)
# when the value changes by more than the amount specified as the second parameter.
# It is possible to reference multiple elements from the hashlist in each FromHashList; if either
# one changes then it will result in the lambda being called.
#
#        obj.update_items = [
#            UpdateManager.FromHashList(["VV_x","VV_y"], 0.01, func(val)
#                                      {
#                                        obj.VV.setTranslation (val.VV_x, val.VV_y + pitch_offset);
#                                      }),
#            UpdateManager.FromHashList(["pitch","roll"], 0.025, func(hdp)
#                                      {
#                                          obj.ladder.setTranslation (0.0, hdp.pitch * pitch_factor+pitch_offset);
#                                          obj.ladder.setCenter (118,830 - hdp.pitch * pitch_factor-pitch_offset);
#                                          obj.ladder.setRotation (-hdp.roll_rad);
#                                          obj.roll_pointer.setRotation (hdp.roll_rad);
#                                      }),
#            props.UpdateManager.FromProperty("velocities/airspeed-kt", 0.01, func(val)
#                                      {
#                                          obj.ias_range.setTranslation(0, val * ias_range_factor);
#                                      }),
#                            props.UpdateManager.FromPropertyHashList(["orientation/alpha-indicated-deg", "orientation/side-slip-deg"], 0.1, func(val)
#                                                                     {
#                                                                         obj.VV_x = val.property["orientation/side-slip-deg"].getValue()*10; # adjust for view
#                                                                         obj.VV_y = val.property["orientation/alpha-indicated-deg"].getValue()*10; # adjust for view
#                                                                         obj.VV.setTranslation (obj.VV_x, obj.VV_y);
#                                                                     }),
#           ]
#
#==== the update loop then becomes ======
#
#        foreach(var update_item; me.update_items)
#        {
#            # hdp is a data provider that can be used as the hashlist for the property
#            # update from hash methods.
#            update_item.update(hdp);
#        }
#
var UpdateManager =
{
    _updateProperty : func(_property)
    {
    },
    FromProperty : func(_propname, _delta, _changed_method)
    {
        var obj = {parents : [UpdateManager] };
        obj.propname = _propname;
        obj.property = props.globals.getNode(_propname);
        obj.delta = _delta;
        obj.curval = obj.property.getValue();
        obj.lastval = obj.curval;
        obj.changed = _changed_method;
        obj.update = func(obj)
        {
            me.curval = me.property.getValue();
            if (me.curval != nil)
            {
                me.localType = me.property.getType();
                if (me.localType == "INT" or me.localType == "LONG" or me.localType == "FLOAT" or me.localType == "DOUBLE")
                  {
                      if (me.lastval == nil or math.abs(me.lastval - me.curval) >= me.delta)
                        {
                            me.lastval = me.curval;
                            me.changed(me.curval);
                        }
                  }
                else if (me.lastval == nil or me.lastval != me.curval)
                  {
                      me.lastval = me.curval;
                      me.changed(me.curval);
                  }
            }
        };
        obj.update(obj);
        return obj;
    },

    IsNumeric : func(hashkey)
    {
        me.localType = me.property[hashkey].getType();
        if (me.localType == "UNSPECIFIED") {
            print("UpdateManager: warning ",hashkey," is ",me.localType, " excluding from update");
            me.property[hashkey] = nil;
        }
        if (me.localType == "INT" or me.localType == "LONG" or me.localType == "FLOAT" or me.localType == "DOUBLE")
          return 1;
        else
          return 0;
    },

    FromPropertyHashList : func(_keylist, _delta, _changed_method)
    {
        var obj = {parents : [UpdateManager] };
        obj.hashkeylist = _keylist;
        obj.delta = _delta;
        obj.lastval = {};
        obj.hashkey = nil;
        obj.changed = _changed_method;
        obj.needs_update = 0;
        obj.property = {};
        obj.is_numeric = {};
        foreach (var hashkey; obj.hashkeylist) {
            obj.property[hashkey] = props.globals.getNode(hashkey);
            obj.lastval[hashkey] = nil;
#            var ty = obj.property[hashkey].getType();
#            if (ty == "INT" or ty == "LONG" or ty == "FLOAT" or ty == "DOUBLE") {
#                obj.is_numeric[hashkey] = 1;
#            } else
#              obj.is_numeric[hashkey] = 0;
#print("create: ", hashkey," ", ty, " isnum=",obj.is_numeric[hashkey]);
#            if (ty == "UNSPECIFIED")
#              print("UpdateManager: warning ",hashkey," is ",ty);
        }
        obj.update = func(obj)
          {
              if (me.lastval == nil)
                  me.needs_update = 1;
              else {
                  me.needs_update = 0;

                  foreach (hashkey; me.hashkeylist) {
                      if (me.property[hashkey] != nil) {
                          me.valIsNumeric = me.IsNumeric(hashkey);

                          if (me.lastval[hashkey] == nil
                              or (me.valIsNumeric and (math.abs(me.lastval[hashkey] - me.property[hashkey].getValue()) >= me.delta))
                              or (!me.valIsNumeric and (me.lastval[hashkey] != me.property[hashkey].getValue()))) {
                              me.needs_update = 1;
                              break;
                          }
                      }
                  }
              }
              if (me.needs_update) {
                  me.changed(me);
                  foreach (hashkey; me.hashkeylist) {
                      me.lastval[hashkey] = me.property[hashkey].getValue();
                  }
              }
          }
        ;
        return obj;
    },
    FromHashValue : func(_key, _delta, _changed_method)
    {
        var obj = {parents : [UpdateManager] };
        obj.hashkey = _key;
        obj.delta = _delta;
        obj.isnum = _delta != nil;
        obj.curval = nil;
        obj.lastval = nil;
        obj.changed = _changed_method;
        obj.update = func(obj)
          {
              me.curval = obj[me.hashkey];
              if (me.curval != nil) {
                  if (me.isnum) {
                      me.curval = num(me.curval);
                      if (me.lastval == nil or math.abs(me.lastval - me.curval) >= me.delta) {
                          me.lastval = me.curval;
                          me.changed(me.curval);
                      }
                  } else {
                      if (me.lastval == nil or me.lastval != me.curval) {
                          me.lastval = me.curval;
                          me.changed(me.curval);
                      }
                  }
              }
          }
        ;
        return obj;
    },
    FromHashList : func(_keylist, _delta, _changed_method)
    {
        var obj = {parents : [UpdateManager] };
        obj.hashkeylist = _keylist;
        obj.delta = _delta;
        obj.lastval = {};
        obj.hashkey = nil;
        obj.changed = _changed_method;
        obj.needs_update = 0;
        obj.isnum = _delta != nil;
        obj.update = func(obj)
          {
              if (me.lastval == nil)
                me.needs_update = 1;
              else
                me.needs_update = 0;

              if (obj != nil or me.lastval == nil) {
                  foreach (var hashkey; me.hashkeylist) {
                      if (me.isnum) {
                          if (me.lastval[hashkey] == nil or math.abs(me.lastval[hashkey] - obj[hashkey]) >= me.delta) {
                              me.needs_update = 1;
                              break;
                          }
                      } elsif (me.lastval[hashkey] == nil or me.lastval[hashkey] != obj[hashkey]) {
                          me.needs_update = 1;
                          break;
                      }
                  }
              }
              if (me.needs_update) {
                  me.changed(obj);
                  foreach (var hashkey; me.hashkeylist) {
                      me.lastval[hashkey] = obj[hashkey];
                  }
              }
          };
        return obj;
    },
};
