
map = {}

map.width = 60
map.height = 30
map.margin_left = 15
map.margin_top = 2

map.food = {}

map.food.x = 10
map.food.y = 10

function map.CreateFood()
    map.food.x = math.random(1,35)
    map.food.y = math.random(1,16)
end

function map.Draw()
    con.SetColor(10)
    map.Print(map.food.x,map.food.y,"&")
end

function map.Begin()
    con.SetColor(7)

    map.Print(0,0,"+")
    map.Print(0,map.height+1,"+")
    map.Print(map.width+1,0,"+")
    map.Print(map.width+1,map.height+1,"+")

   for x = 1,map.width-1,1
   do
    map.Print(x,0,"-")
    map.Print(x,map.height+1,"-")
   end

   for y = 1,map.height,1
   do
    map.Print(0,y,"|")
    map.Print(map.width+1,y,"|")
   end
end

function map.Print(x,y,c)
con.Gotoxy(map.margin_left+x,map.margin_top+y)
con.Print(c)
end