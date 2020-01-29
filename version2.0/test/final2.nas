###############################################################################
##
## Nasal module for dual control over the multiplayer network.
##
##  Copyright (C) 2007 - 2010  Anders Gidenstam  (anders(at)gidenstam.org)
##  This file is licensed under the GPL license version 2 or later.
##
###############################################################################
## MP properties
var lat_mpp     = "position/latitude-deg";
var lon_mpp     = "position/longitude-deg";
var alt_mpp     = "position/altitude-ft";
var heading_mpp = "orientation/true-heading-deg";
var pitch_mpp   = "orientation/pitch-deg";
var roll_mpp    = "orientation/roll-deg";

# Import components from the mp_broadcast module.
var Binary         = mp_broadcast.Binary;
var MessageChannel = mp_broadcast.MessageChannel;

###############################################################################
# Utility classes

############################################################
# Translate a property into another.
#  Factor and offsets are only used for numeric values.
#   src    - source      : property node
#   dest   - destination : property node
#   factor -             : double
#   offset -             : double
var Translator = {};
Translator.new = func (src = nil, dest = nil, factor = 1, offset = 0) {
  var obj = { parents   : [Translator],
              src       : src,
              dest      : dest,
              factor    : factor,
              offset    : offset };
  if (obj.src == nil or obj.dest == nil) {
    print("Translator[");
    print("  ", debug.string(obj.src));
    print("  ", debug.string(obj.dest));
    print("]");
    fail();
  }

  return obj;
}
Translator.update = func () {
  var v = me.src.getValue();
  if (is_num(v)) {
    me.dest.setValue(me.factor * v + me.offset);
  } else {
    if (typeof(v) == "scalar")
      me.dest.setValue(v);
  }
}

############################################################
# Detects flanks on two insignals encoded in a property.
# - positive signal up/down flank
# - negative signal up/down flank
#   n                 - source : property node
#   on_positive_flank - action : func (v)
#   on_negative_flank - action : func (v)
var EdgeTrigger = {};
EdgeTrigger.new = func (n, on_positive_flank, on_negative_flank) {
  var obj = { parents   : [EdgeTrigger],
              old       : 0,
              node      : n, 
              pos_flank : on_positive_flank,
              neg_flank : on_negative_flank };
  if (obj.node == nil) {
    print("EdgeTrigger[");
    print("  ", debug.string(obj.node));
    print("]");
    fail();
  }
  return obj;
}
EdgeTrigger.update = func {
  # NOTE: float MP properties get interpolated.
  #       This detector relies on that steady state is reached between
  #       flanks.
  var val = me.node.getValue();
  if (!is_num(val)) return;
  if (me.old == 1) {
    if (val < me.old) {
      me.pos_flank(0);
    }
  } elsif (me.old == 0) {
    if (val > me.old) {
      me.pos_flank(1);
    } elsif (val < me.old) {
      me.neg_flank(1);
    }
  } elsif (me.old == -1) {
    if (val > me.old) {
      me.neg_flank(0);
    }
  }
  me.old = val;
}

############################################################
# StableTrigger: Triggers an action when a MPP property
#                becomes stable (i.e. doesn't change for
#                MIN_STABLE seconds).
#   src     - MP prop : property node
#   action  - action to take when the value becomes stable : [func(v)]
# An action is triggered when value has stabilized.
var StableTrigger = {};
StableTrigger.new = func (src, action) {
  var obj = { parents      : [StableTrigger],
              src          : src,
              action       : action,
              old          : 0,
              stable_since : 0,
              wait         : 0,
              MIN_STABLE   : 0.01 };
  # Error checking.
  var bad = (obj.src == nil) or (action = nil);

  if (bad) {
    print("StableTrigger[");
    print("  ", debug.string(obj.src));
    print("  ", debug.string(obj.action));
    print("]");
    fail();
  }

  return obj;
}
StableTrigger.update = func () {
  var v   = me.src.getValue();
  if (!is_num(v)) return;
  var t = getprop("/sim/time/elapsed-sec"); # NOTE: simulated time.

  if ((me.old == v) and
      ((t - me.stable_since) > me.MIN_STABLE) and (me.wait == 1)) {
    # Trigger action.
    me.action(v);

    me.wait = 0;
  } elsif (me.old == v) {
    # Wait. This is either before the signal is stable or after the action. 
  } else {
    me.stable_since = t;
    me.wait         = 1;
    me.old          = me.src.getValue();
  }
}

############################################################
# Selects the most recent value of two properties.
#   src1      -  : property node
#   src2      -  : property node
#   dest      -  : property node
#   threshold -  : double
var MostRecentSelector = {};
MostRecentSelector.new = func (src1, src2, dest, threshold) {
  var obj = { parents   : [MostRecentSelector],
              old1      : 0,
              old2      : 0,
              src1      : src1,
              src2      : src2,
              dest      : dest,
              thres     : threshold };
  if (obj.src1 == nil or obj.src2 == nil or obj.dest == nil) {
    print("MostRecentSelector[");
    print("  ", debug.string(obj.src1));
    print("  ", debug.string(obj.src2));
    print("  ", debug.string(obj.dest));
    print("]");
  }

  return obj;
}
MostRecentSelector.update = func {
  var v1 = me.src1.getValue();
  var v2 = me.src2.getValue();
  if (!is_num(v1) and !is_num(v2)) return;
  elsif (!is_num(v1)) me.dest.setValue(v2);
  elsif (!is_num(v2)) me.dest.setValue(v1);
  else {
      if (abs (v2 - me.old2) > me.thres) {
          me.old2 = v2;
          me.dest.setValue(me.old2);
      }
      if (abs (v1 - me.old1) > me.thres) {
          me.old1 = v1;
          me.dest.setValue(me.old1);
      }
  }
}

############################################################
# Adds two input properties.
#   src1      -  : property node
#   src2      -  : property node
#   dest      -  : property node
var Adder = {};
Adder.new = func (src1, src2, dest) {
  var obj = { parents : [DeltaAccumulator],
              src1    : src1,
              src2    : src2,
              dest    : dest };
  if (obj.src1 == nil or obj.src2 == nil or obj.dest == nil) {
    print("Adder[");
    print("  ", debug.string(obj.src1));
    print("  ", debug.string(obj.src2));
    print("  ", debug.string(obj.dest));
    print("]");
    fail();
  }

  return obj;
}
Adder.update = func () {
  var v1 = me.src1.getValue();
  var v2 = me.src2.getValue();
  if (!is_num(v1) or !is_num(v2)) return;
  me.dest.setValue(v1 + v2);
}

############################################################
# Adds the delta of src to dest.
#   src       -  : property node
#   dest      -  : property node
var DeltaAdder = {};
DeltaAdder.new = func (src, dest) {
  var obj = { parents : [DeltaAdder],
              old     : 0,
              src     : src,
              dest    : dest };
  if (obj.src == nil or obj.dest == nil) {
    print("DeltaAdder[", debug.string(obj.src), ", ",
          debug.string(obj.dest), "]");
    fail();
  }

  return obj;
}
DeltaAdder.update = func () {
  var v = me.src.getValue();
  if (!is_num(v)) return;
  me.dest.setValue((v - me.old) + me.dest.getValue());
  me.old = v;
}

############################################################
# Switch encoder: Encodes upto 32 boolean properties in one
# int property.
#   inputs    - list of property nodes
#   dest      - where the bitmask is stored : property node
var SwitchEncoder = {};
SwitchEncoder.new = func (inputs, dest) {
  var obj = { parents : [SwitchEncoder],
              inputs  : inputs,
              dest    : dest };
  # Error checking.
  var bad = (obj.dest == nil);
  foreach (var i; inputs) {
    if (i == nil) { bad = 1; }
  }

  if (bad) {
    print("SwitchEncoder[");
    foreach (var i; inputs) {
      print("  ", debug.string(i));
    }
    print("  ", debug.string(obj.dest));
    print("]");
    fail();
  }

  return obj;
}
SwitchEncoder.update = func () {
  var v = 0;
  var b = 1;
  forindex (var i; me.inputs) {
    if (me.inputs[i].getBoolValue()) {
      v = v + b;
    }
    b *= 2;
  }
  me.dest.setIntValue(v);
}

############################################################
# Switch decoder: Decodes a bitmask in an int property.
#   src     -                 : property node
#   actions - list of actions : [func(b)]
# Actions are triggered when their input bit change.
# Due to interpolation the decoder needs to wait for a
# stable input value.
var SwitchDecoder = {};
SwitchDecoder.new = func (src, actions) {
  var obj = { parents : [SwitchDecoder],
              wait         : 0,
              old          : 0,
              old_stable   : 0,
              stable_since : 0,
              reset        : 1,
              src          : src,
              actions      : actions,
              MIN_STABLE   : 0.1 };
  # Error checking.
  var bad = (obj.src == nil);
  foreach (var a; obj.actions) {
    if (a == nil) { bad = 1; }
  }
  
  if (bad) {
    print("SwitchDecoder[");
    print("  ", debug.string(obj.src));
    foreach (var a; obj.actions) {
      print("  ", debug.string(a));
    }
    print("]");
    fail();
  }

  return obj;
}
SwitchDecoder.update = func () {
  var t = getprop("/sim/time/elapsed-sec"); # NOTE: simulated time.
  var v = me.src.getValue();
  if (!is_num(v)) return;

  if ((me.old == v) and ((t - me.stable_since) > me.MIN_STABLE) and
      (me.wait == 1)) {
    var ov = me.old_stable;
# Use this to improve.
#<cptf> here's the boring version:  var bittest = func(u, b) { while (b) { u = int(u / 2); b -= 1; } u != int(u / 2) * 2; }
    forindex (var i; me.actions) {
      var m  = math.mod(v, 2);
      var om = math.mod(ov, 2);
      if ((m != om or me.reset)) { me.actions[i](m?1:0); }
      v  = (v - m)/2;
      ov = (ov - om)/2;
    }
    me.old_stable = me.src.getValue();
    me.wait  = 0;
    me.reset = 0;
  } elsif (me.old == v) {
    # Wait. This is either before the bitmask is stable or after
    # it has been processed. 
  } else {
    me.stable_since = t;
    me.wait         = 1;
    me.old          = me.src.getValue();
  }
}

############################################################
# Time division multiplexing encoder: Transmits a list of
# properties over a MP enabled string property.
#   inputs  - input properties : [property node]
#   dest    - MP string prop   : property node
# Note: TDM can have high latency so it is best used for
# non-time critical properties.
var TDMEncoder = {};
TDMEncoder.new = func (inputs, dest) {
  var obj = { parents   : [TDMEncoder],
              inputs    : inputs,
              channel   : MessageChannel.new(dest,
                                             func (msg) {
                                               print("This should not happen!");
                                             }),
              MIN_INT   : 0.25,
              last_time : 0,
              next_item : 0,
              old       : [] };
  # Error checking.
  var bad = (dest == nil) or (obj.channel == nil);
  foreach (var i; inputs) {
    if (i == nil) { bad = 1; }
  }

  if (bad) {
    print("TDMEncoder[");
    foreach (var i; inputs) {
      print("  ", debug.string(i));
    }
    print("  ", debug.string(dest));
    print("]");
  }

  setsize(obj.old, size(obj.inputs));

  return obj;
}
TDMEncoder.update = func () {
  var t = getprop("/sim/time/elapsed-sec"); # NOTE: simulated time.
  if (t > me.last_time + me.MIN_INT) {
    var n = size(me.inputs);
    while (1) {
      var v = me.inputs[me.next_item].getValue();

      if ((n <= 0) or (me.old[me.next_item] != v)) {
        # Set the MP properties to send the next item.
        me.channel.send(Binary.encodeByte(me.next_item) ~
                        Binary.encodeDouble(v));

        me.old[me.next_item] = v;

        me.last_time = t;
        me.next_item += 1;
        if (me.next_item >= size(me.inputs)) { me.next_item = 0; }
        return;
      } else {
        # Search for changed property.
        n -= 1;
        me.next_item += 1;
        if (me.next_item >= size(me.inputs)) { me.next_item = 0; }
      }         
    }
  }
}

############################################################
# Time division multiplexing decoder: Receives a list of
# properties over a MP enabled string property.
#   src     - MP string prop  : property node
#   actions - list of actions : [func(v)]
# An action is triggered when its value is received.
# Note: TDM can have high latency so it is best used for
# non-time critical properties.
var TDMDecoder = {};
TDMDecoder.new = func (src, actions) {
  var obj = { parents      : [TDMDecoder],
              actions      : actions };
  obj.channel = MessageChannel.new(src,
                                   func (msg) {
                                     obj.process(msg);
                                   });

  # Error checking.
  var bad = (src == nil) or (obj.channel == nil);
  foreach (var a; actions) {
    if (a == nil) { bad = 1; }
  }

  if (bad) {
    print("TDMDecoder[");
    print("  ", debug.string(src));
    foreach (var a; actions) {
      print("  ", debug.string(a));
    }
    print("]");
    fail();
  }

  return obj;
}
TDMDecoder.process = func (msg) {
  var v1 = Binary.decodeByte(msg);
  var v2 = Binary.decodeDouble(substr(msg, 1));
  # Trigger action.
  me.actions[v1](v2);
}
TDMDecoder.update = func {
  me.channel.update();
}

###############################################################################
# Internal utility functions

var is_num = func (v) {
    return num(v) != nil;
}

# fail causes a Nasal runtime error so we get a backtrace.
var fail = func {
    error_detected_in_calling_context();
}

###############################################################################

###############################################################################
# Copilot selection dialog.
#
# Usage: dual_control_tools.copilot_dialog.show(<copilot type string>);
#
var COPILOT_DLG = 0;
var copilot_dialog = {};
############################################################
copilot_dialog.init = func (copilot_type, x = nil, y = nil) {
    me.x = x;
    me.y = y;
    me.bg = [0, 0, 0, 0.3];    # background color
    me.fg = [[1.0, 1.0, 1.0, 1.0]]; 
    #
    # "private"
    if (contains(aircraft_dual_control, "copilot_view")) {
        me.title = "Pilot selection";
    } else {
        me.title = "Copilot selection";
    }
    me.basenode = props.globals.getNode("sim/remote", 1);
    me.dialog = nil;
    me.namenode = props.Node.new({"dialog-name" : me.title });
    me.listeners = [];
    me.copilot_type = copilot_type;
}
############################################################
copilot_dialog.create = func {
    if (me.dialog != nil)
        me.close();

    me.dialog = gui.Widget.new();
    me.dialog.set("name", me.title);
    if (me.x != nil)
        me.dialog.set("x", me.x);
    if (me.y != nil)
        me.dialog.set("y", me.y);

    me.dialog.set("layout", "vbox");
    me.dialog.set("default-padding", 0);
    var titlebar = me.dialog.addChild("group");
    titlebar.set("layout", "hbox");
    titlebar.addChild("empty").set("stretch", 1);
    if (contains(aircraft_dual_control, "copilot_view")) {
        titlebar.addChild("text").set("label", "Book your flight");
    } else {
        titlebar.addChild("text").set("label", "Passengers online");
    }
    var w = titlebar.addChild("button");
    w.set("pref-width", 16);
    w.set("pref-height", 16);
    w.set("legend", "");
    w.set("default", 0);
    w.set("key", "esc");
    w.setBinding("nasal", "dual_control_tools.copilot_dialog.destroy(); ");
    w.setBinding("dialog-close");
    me.dialog.addChild("hrule");

    var content = me.dialog.addChild("group");
    content.set("layout", "vbox");
    content.set("halign", "center");
    content.set("default-padding", 5);

    # Generate the dialog contents.
    me.players = me.find_copilot_players();
    var i = 0;
    var tmpbase  = me.basenode.getNode("dialog", 1);
    var selected = me.basenode.getNode("pilot-callsign").getValue();
    foreach (var p; me.players) {
        var tmp = tmpbase.getNode("b[" ~ i ~ "]", 1);
        tmp.setBoolValue(streq(selected, p));
        var w = content.addChild("checkbox");
        w.node.setValues({"label"    : p,
                          "halign"   : "left",
                          "property" : tmp.getPath()});
        w.setBinding
            ("nasal",
             "dual_control_tools.copilot_dialog.select_action(" ~ i ~ ");");
        i = i + 1;
    }
    me.dialog.addChild("hrule");

    # Display the dialog.
    fgcommand("dialog-new", me.dialog.prop());
    fgcommand("dialog-show", me.namenode);
}
############################################################
copilot_dialog.close = func {
    fgcommand("dialog-close", me.namenode);
}
############################################################
copilot_dialog.destroy = func {
    COPILOT_DLG = 0;
    me.close();
    foreach(var l; me.listeners)
        removelistener(l);
    delete(gui.dialog, "\"" ~ me.title ~ "\"");
}
############################################################
copilot_dialog.show = func (copilot_type) {
#    print("Showing MPCopilots dialog!");
    if (!COPILOT_DLG) {
        COPILOT_DLG = int(getprop("/sim/time/elapsed-sec"));
        me.init(copilot_type);
        me.create();
        me._update_(COPILOT_DLG);
    }
}
############################################################
copilot_dialog._redraw_ = func {
    if (me.dialog != nil) {
        me.close();
        me.create();
    }
}
############################################################
copilot_dialog._update_ = func (id) {
    if (COPILOT_DLG != id) return;
    me._redraw_();
    settimer(func { me._update_(id); }, 4.1);
}
############################################################
copilot_dialog.select_action = func (n) {
    var selected = me.basenode.getNode("pilot-callsign").getValue();
    var bs = me.basenode.getNode("dialog").getChildren();
    # Assumption: There are two true b:s or none. The one not matching selected
    #             is the new selection.
    var i = 0;
    me.basenode.getNode("pilot-callsign").setValue("");
    foreach (var b; bs) {
        if (!b.getValue() and (i == n)) {
            b.setValue(1);
            me.basenode.getNode("pilot-callsign").setValue(me.players[i]);
        } else {
            b.setValue(0);
        }
        i = i + 1;
    }
    dual_control.main.reset();
    me._redraw_();
}
############################################################
# Return a list containing all nearby copilot players of the right type.
copilot_dialog.find_copilot_players = func {
    var mpplayers =
        props.globals.getNode("ai/models").getChildren("multiplayer");

    var res = [];
    foreach (var pilot; mpplayers) {
        if ((pilot.getNode("valid") != nil) and
            (pilot.getNode("valid").getValue()) and
            (pilot.getNode("sim/model/path") != nil)) {
            var type = pilot.getNode("sim/model/path").getValue();

            if (type == me.copilot_type) {
                append(res, pilot.getNode("callsign").getValue());
            }
        }
    }
#    debug.dump(res);
    return res; 
}
###############################################################################
