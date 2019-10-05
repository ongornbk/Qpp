
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

function column.Draw(this,map)

if this.x < this.width
then

    for y = map.margin_top,this.y - map.offset,1
    do
        conio.SetColor(Image.get(background,this.x+this.width,y-map.margin_top))
        conio.Set(this.x+this.width,y," ")
    end

    for y = this.y + map.offset,map.sizey,1
    do
        conio.SetColor(Image.get(background,this.x+this.width,y-map.margin_top))
        conio.Set(this.x+this.width,y," ")
    end

else

    for y = map.margin_top,this.y - map.offset,1 -- Top Column
do
    if this.x+this.width < map.sizex
    then
    conio.SetColor(Image.get(background,this.x+this.width-1,y-map.margin_top))
    conio.Set(this.x+this.width,y," ")
    end
    conio.SetColor(map.color)
    conio.Set(this.x,y," ")
end

for y = this.y + map.offset,map.sizey,1 -- Bottom Column
do
    if this.x+this.width < map.sizex
    then
    conio.SetColor(Image.get(background,this.x+this.width-1,y-map.margin_top))
    conio.Set(this.x+this.width,y," ")
    end
    conio.SetColor(map.color)
    conio.Set(this.x,y," ")
end

end
end