var smartScreen = canvas.new({
  "name": "smartScreen",   # The name is optional but allow for easier identification
  "size": [2048, 2048], # Size of the underlying texture (should be a power of 2, required) [Resolution]
  "view": [768, 768],  # Virtual resolution (Defines the coordinate system of the canvas [Dimensions]
                        # which will be stretched the size of the texture, required)
  "mipmapping": 1       # Enable mipmapping (optional)
});

smartScreen.addPlacement({"node": "screen", "texture": "screen.jpeg"});
var group = smartScreen.createGroup();

# Create a text element and set some values
var text = group.createChild("text", "optional-id-for element")
          .setTranslation(10, 20)      # The origin is in the top left corner
          .setAlignment("left-center") # All values from osgText are supported (see $FG_ROOT/Docs/README.osgtext)
          .setFont("LiberationFonts/LiberationSans-Regular.ttf") # Fonts are loaded either from $AIRCRAFT_DIR/Fonts or $FG_ROOT/Fonts
          .setFontSize(14, 1.2)        # Set fontsize and optionally character aspect ratio
          .setColor(1,0,0)             # Text color
          .setText("This is a text element");
text.hide();
text.setText("SELF TEST NORMAL").show();



var ui_root = smartScreen.createGroup();
var vbox = canvas.VBoxLayout.new();
smartScreen.setLayout(vbox);


var button_onl = canvas.gui.widgets.Button.new(ui_root, canvas.style, {}).setText("Online OSM").listen("clicked", func showOnlineMap());
var button_offl = canvas.gui.widgets.Button.new(ui_root, canvas.style, {}).setText("Offline OSM").listen("clicked", func showOfflineMap());
button_onl.setSizeHint([32, 128]);
button_offl.setSizeHint([32, 128]);

var label = canvas.gui.widgets.Label.new(ui_root, canvas.style, {});

var button_box = canvas.HBoxLayout.new();
button_box.addItem(button_onl);
button_box.addItem(button_offl);
button_box.addItem(label);
button_box.addStretch(1);

vbox.addItem(button_box);
vbox.addStretch(1);


var showOnlineMap = func(){
  TestMap.show();
  g.hide();
  label.setText("Online Mode");
}


var showOfflineMap = func(){
  TestMap.hide();
  g.show();
  label.setText("Offline Mode");
}


# Online Map using MapStructure
var TestMap = smartScreen.createGroup().createChild("map");
TestMap.setTranslation(smartScreen.get("view[0]")/2,smartScreen.get("view[1]")/2);


var ctrl_ns = canvas.Map.Controller.get("Aircraft position");
var source = ctrl_ns.SOURCES["map-dialog"];
if (source == nil) {
  # TODO: amend
  var source = ctrl_ns.SOURCES["map-dialog"] = {
    getPosition: func subvec(geo.aircraft_position().latlon(), 0, 2),# ? ? ?
    getAltitude: func getprop('/position/altitude-ft'),
    getHeading:  func {
      if (me.aircraft_heading)
        getprop('/orientation/heading-deg');
      else
        0;
    },
    aircraft_heading: 1,
  };
}
setlistener("/sim/gui/dialogs/map-canvas/aircraft-heading-up", func(n){source.aircraft_heading = n.getBoolValue();}, 1);
TestMap.setController("Aircraft position", "map-dialog");
TestMap.setRange(1); 

var r = func(name,vis=1,zindex=nil){return caller(0)[0];};
# TODO: we'll need some z-indexing here, right now it's just random

foreach(var type; [r('APS')] ){
    TestMap.addLayer(factory: canvas.SymbolLayer, type_arg: type.name, visible: type.vis, priority: 2);
}

foreach(var type; [ r('OSM')]) {
    TestMap.addLayer(factory: canvas.OverlayLayer,
                    type_arg: type.name,
                     visible: type.vis,
                    priority: 1);
}


TestMap.hide();

# Offline map

var g = smartScreen.createGroup();
var zoom = 15;
var type = "intl";
var tile_size = 256;


var changeZoom = func(d)
{
  zoom = math.max(2, math.min(19, zoom + d));
  updateTiles();
}

# http://polymaps.org/docs/
# https://github.com/simplegeo/polymaps
# https://github.com/Leaflet/Leaflet

var maps_base = getprop("/sim/fg-home") ~ '/cache/maps';

var makePath =
string.compileTemplate(maps_base ~ '/osm-{type}/{z}/{x}/{y}.jpg');
var num_tiles = [4, 4];

var center_tile_offset = [(num_tiles[0]-1)/2, (num_tiles[1]-1)/ 2];

# simple aircraft icon at current position/center of the map
g.createChild("path")
  .moveTo( tile_size*center_tile_offset[0]-10, tile_size*center_tile_offset[1])
  .horiz(20)
  .move(-10,-10)
  .vert(20)
  .set("stroke", "red")
  .set("stroke-width", 2)
  .set("z-index", 1);

# initialize the map by setting up
# a grid of raster images  

var tiles = setsize([], num_tiles[0]);
for(var x=0; x<num_tiles[0]; x+=1)
{
  tiles[x] = setsize([], num_tiles[1]);
  for(var y=0; y<num_tiles[1]; y+=1)
    tiles[x][y] = g.createChild("image", "map-tile");
}

var last_tile = [-1,-1];
var last_type = type;

# this is the callback that will be regularly called by the timer
# to update the map
var updateTiles = func()
{
  # get current position
  var lat = getprop('/position/latitude-deg');
  var lon = getprop('/position/longitude-deg');

  var n = math.pow(2, zoom);
  var offset = [n*((lon+180)/360)-center_tile_offset[0], (1-math.ln(math.tan(lat*math.pi/180)+1/math.cos(lat*math.pi/180))/math.pi)/2*n-center_tile_offset[1]];
  var tile_index = [int(offset[0]), int(offset[1])];

  var ox = tile_index[0] - offset[0];
  var oy = tile_index[1] - offset[1];

  for(var x = 0; x < num_tiles[0]; x += 1)
    for(var y = 0; y < num_tiles[1]; y += 1)
      tiles[x][y].setTranslation(int((ox + x) * tile_size + 0.5), int((oy + y) * tile_size + 0.5));

  if(  tile_index[0] != last_tile[0]
    or tile_index[1] != last_tile[1]
    or type != last_type )
  {
    for(var x = 0; x < num_tiles[0]; x += 1)
      for(var y = 0; y < num_tiles[1]; y += 1)
      {
        var pos = {
          z: zoom,
          x: int(offset[0] + x),
          y: int(offset[1] + y),
          type: type
        };

        (func {
            var img_path = makePath(pos);
            var tile = tiles[x][y];
            print('loading ' ~ img_path);
            tile.set("src", img_path);
          })();
        # lambda
      }

      last_tile = tile_index;
      last_type = type;
  }
};

# set up a timer that will invoke updateTiles at 2-second intervals
var update_timer = maketimer(2, updateTiles);
# actually start the timer
update_timer.start();

# set up default zoom level
changeZoom(0);

#g.hide();


