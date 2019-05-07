
column = {}

function column.new(_x,_width)
return
{
rendering = false,
x = _x,
y = 0,
width = _width
}
end

function column.Draw(this,color,default_color,offset,max,top)

if this.x < this.width
then

    for y = top,this.y - offset,1
    do
        conio.Gotoxy(this.x+this.width,y)
        conio.SetColor(default_color)
        conio.Print(" ")
    end

    for y = this.y + offset,max,1
    do
        conio.Gotoxy(this.x+this.width,y)
        conio.SetColor(default_color)
        conio.Print(" ")
    end


else

    for y = top,this.y - offset,1
do
    conio.Gotoxy(this.x+this.width,y)
    conio.SetColor(default_color)
    conio.Print(" ")
    conio.Gotoxy(this.x,y)
    conio.SetColor(color)
    conio.Print(" ")
end

for y = this.y + offset,max,1
do
    conio.Gotoxy(this.x+this.width,y)
    conio.SetColor(default_color)
    conio.Print(" ")
    conio.Gotoxy(this.x,y)
    conio.SetColor(color)
    conio.Print(" ")
end

end
end