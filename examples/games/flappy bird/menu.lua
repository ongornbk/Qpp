
Menu = {}

function Menu.new()
    return
    {
size = 0,
options = {}
    }
end

function Menu.PushOption(this,name)
    this.options[this.size] = name
    this.size = this.size + 1
end

function Menu.Select(this)

conio.Gotoxy(0,0)
conio.SetColor(15)

for i = 0,this.size-1,1
do
conio.Println(this.options[i])
end

local guard = true
local lock = 0
local pointer = 0

while guard
do
conio.Gotoxy(string.len(this.options[pointer]),pointer)
conio.Print(" <--")

if(win.KeyDown(0x26))
then

while (win.KeyDown(0x26))
do
sleep(500000)
end

if pointer > 0
then
conio.Gotoxy(string.len(this.options[pointer]),pointer)
conio.Print("    ")
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
conio.Gotoxy(string.len(this.options[pointer]),pointer)
conio.Print("    ")
pointer = pointer + 1
end
end

if(win.KeyDown(0x0D))
then
    while (win.KeyDown(0x0D))
do
sleep(500000)
end
return pointer
end

end
end