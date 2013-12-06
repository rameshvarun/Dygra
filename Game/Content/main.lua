level_list = { 'level1.xml', 'level2.xml', 'level3.xml', 'level4.xml' }

require("wx")

--Splits a string given some delimiter
function split(str, pat)
   local t = {}  -- NOTE: use {n = 0} in Lua-5.0
   local fpat = "(.-)" .. pat
   local last_end = 1
   local s, e, cap = str:find(fpat, 1)
   while s do
      if s ~= 1 or cap ~= "" then
	 table.insert(t,cap)
      end
      last_end = e+1
      s, e, cap = str:find(fpat, last_end)
   end
   if last_end <= #str then
      cap = str:sub(last_end)
      table.insert(t, cap)
   end
   return t
end

function startGame( event )
	print("Game is starting...")
	
	--Get dimensions
	dims = split( res_select:GetValue(), "x" )
	w = tonumber(dims[1])
	h = tonumber(dims[2])
	
	--Get fullscreen
	f = full_select:GetValue()
	
	--Get raytracer type
	rtype = rend_select:GetValue()
	
	frame:Close(true)
	
	initialize( w, h, f)
	
	for _, level_file in ipairs(level_list) do
		level1 = Level( rtype )
		level1:LoadXML( level_file )
		level1:BuildShader()
		local level_result = level1:run()
		
		if level_result == "exit" then
			break
		end
	end
end

--Graphical Settings Dialog
frame = wx.wxFrame(wx.NULL, wx.wxID_ANY, "Dygra Graphics Settings",  wx.wxDefaultPosition, wx.wxSize(450, 450), wx.wxDEFAULT_FRAME_STYLE)
panel = wx.wxPanel(frame, wx.wxID_ANY)

sizer = wx.wxFlexGridSizer(4, 2, 5, 5)

--Row 1
res_text = wx.wxStaticText(panel, -1, "Resolution: ")
sizer:Add(res_text , 0, wx.wxALL, 10)

res_select = wx.wxComboBox(panel, -1, "640 x 480")
res_select:Append("320 x 240")
res_select:Append("640 x 480")
res_select:Append("800 x 600")
res_select:Append("1280 x 720")
res_select:Append("1600 x 900")
res_select:Append("1920 x 1080")
sizer:Add(res_select , 0, wx.wxALL, 10)

--Row 2
sizer:Add( wx.wxStaticText(panel, -1, "") , 0, wx.wxALL, 10)

full_select = wx.wxCheckBox(panel, -1, "Fullscreen")
full_select:SetValue(true)
sizer:Add(full_select , 0, wx.wxALL, 10)

--Row 3
sizer:Add( wx.wxStaticText(panel, -1, "Renderer Type (Only change this\n if the default doesn't work)") , 0, wx.wxALL, 10)

rend_select = wx.wxComboBox(panel, -1, "raytracing")
rend_select:Append("raytracing")
rend_select:Append("raymarching")
sizer:Add(rend_select , 0, wx.wxALL, 10)

--Row 4
sizer:Add( wx.wxStaticText(panel, -1, "") , 0, wx.wxALL, 10)

run_button = wx.wxButton(panel, -1, "Start Game")
sizer:Add(run_button , 0, wx.wxALL, 10)

frame:Connect(run_button:GetId() , wx.wxEVT_COMMAND_BUTTON_CLICKED, startGame)

panel:SetSizer(sizer)
panel:Layout()

panel:Fit()
frame:Fit()
  
frame:Centre()
frame:Show(true)

wx.wxGetApp():MainLoop()
