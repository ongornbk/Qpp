Sunflower = {}

Sunflower.window = 0
Sunflower.hwnd = 0
Sunflower.lpmsg = 0
Sunflower.msg = 0
Sunflower.wparam = 0
Sunflower.lparam = 0

function Sunflower.Start(title,show)
Sunflower.lpmsg = malloc(48)
bzero(Sunflower.lpmsg)
Sunflower.window = windows.CreateWindow()
Sunflower.hwnd = windows.GetWindowHandle(Sunflower.window)
windows.SetTitle(Sunflower.hwnd,title)
windows.Show(Sunflower.hwnd,show)
end

function Sunflower.Run()
local val = windows.GetMessage(Sunflower.hwnd,Sunflower.lpmsg)
--Sunflower.lparam,Sunflower.wparam,Sunflower.msg = windows.RetrieveMessage(Sunflower.lpmsg)
return val
end

function Sunflower.Peek()
--local val = PeekMessage()
--Sunflower.lparam,Sunflower.wparam,Sunflower.msg = RetrieveMessage()
--return val
end

function Sunflower.Proc()
windows.Proc(Sunflower.hwnd,Sunflower.msg,Sunflower.wparam,Sunflower.lparam)
end

function Sunflower.Close()
windows.DestroyWindow(Sunflower.window)
end