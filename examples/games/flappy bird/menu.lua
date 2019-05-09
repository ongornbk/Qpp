
require "MenuItem"

Menu = {}

function Menu.new()
    return
    {
size = 0,
options = {}
    }
end

function Menu.PushOption(this,name)
    this.options[this.size] = MenuItem.new()
    this.options[this.size].name = name
    this.size = this.size + 1
end

function Menu.Select(this)

conio.Gotoxy(0,0)
conio.SetColor(15)

for i = 0,this.size-1,1
do
MenuItem.Select(this.options[i])
MenuItem.Draw(this.options[i])
end

local guard = true
local lock = 0
local pointer = 0

while guard
do

MenuItem.Select(this.options[pointer])
conio.Gotoxy(string.len(this.options[pointer].name),pointer)
MenuItem.DrawPtr(this.options[pointer])

if(win.KeyDown(0x26))
then

while (win.KeyDown(0x26))
do
sleep(500000)
end

if pointer > 0
then
conio.Gotoxy(string.len(this.options[pointer].name),pointer)
MenuItem.Clean(this.options[pointer])
pointer = pointer - 1
end
end

if(win.KeyDown(0x28))
then

while (win.KeyDown(0x28))
do
sleep(500000)
end

if pointer < this.size - 1
then
conio.Gotoxy(string.len(this.options[pointer].name),pointer)
MenuItem.Clean(this.options[pointer])
pointer = pointer + 1
end
end

if(win.KeyDown(0x25))
then

while (win.KeyDown(0x25))
do
sleep(500000)
end

MenuItem.Decrement(this.options[pointer])
end


if(win.KeyDown(0x27))
then

while (win.KeyDown(0x27))
do
sleep(500000)
end

MenuItem.Increment(this.options[pointer])
end


if(win.KeyDown(0x0D))
then
    while (win.KeyDown(0x0D))
do
sleep(500000)
--for i = 0,this.size-1,1
--do
--MenuItem.release(this.options[i])
--end
end
return pointer
end

end
end

function Menu.SetItemReference(this,reference)
this.options[this.size-1].ref = reference
end

function Menu.MakeItemSelectable(this)
    this.options[this.size-1].selectable = true
end

function Menu.PushItem(this,item)
MenuItem.PushItem(this.options[this.size-1],item)
end

function Menu.SetDefault(this,ptr)
this.options[this.size-1].selection = ptr
end