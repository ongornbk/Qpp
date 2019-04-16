Snake = {}

Snake.element = {}

Snake.element[0] = {}

Snake.element[0].x = 5
Snake.element[0].y = 5

Snake.size = 0

Snake.lastposition = {}

Snake.lastposition.x = 5
Snake.lastposition.y = 5

Snake.food = {}

Snake.food.x = 10
Snake.food.y = 10

function Snake.CreateFood()
    Snake.food.x = math.random(1,35)
    Snake.food.y = math.random(1,16)
end

function Snake.Enlarge ()
    Snake.size = Snake.size + 1
    Snake.element[Snake.size] = {}
    Snake.element[Snake.size].x = Snake.element[Snake.size-1].x
    Snake.element[Snake.size].y = Snake.element[Snake.size-1].y
  end
  
function Snake.Collision(x,y)

    local collision = 0
for i = 1,Snake.size,1
do

    if(x == Snake.element[i].x and y == Snake.element[i].y)
    then
        collision = i
        break;
    end
end

if(collision > 0)
then
for i = collision,Snake.size,1
do
    con.Gotoxy(margin_left+Snake.element[i].x,margin_top+Snake.element[i].y)
    con.Print(" ")
end

Snake.size = collision

end

end

function Snake.Draw()

    con.Gotoxy(margin_left+Snake.food.x,margin_top+Snake.food.y)
    con.Print("&")

    con.Gotoxy(margin_left+Snake.element[0].x,margin_top+Snake.element[0].y)
    con.Print(head)

for i = 1,Snake.size-1,1
do
    con.Gotoxy(margin_left+Snake.element[i].x,margin_top+Snake.element[i].y)
    if(Snake.element[i].x == Snake.food.x and Snake.element[i].y == Snake.food.y)
    then
        con.Print("X")
    else
        con.Print(body)
    end

end

    if(Snake.size > 0)
    then
    con.Gotoxy(margin_left+Snake.element[Snake.size].x,margin_top+Snake.element[Snake.size].y)
    con.Print(tail)
    end

    con.Gotoxy(margin_left+Snake.lastposition.x,margin_top+Snake.lastposition.y)
    con.Print(" ")
end

function Snake.Update(offset_x,offset_y)

Snake.lastposition.x = Snake.element[Snake.size].x
Snake.lastposition.y = Snake.element[Snake.size].y

for i = 0,Snake.size-1,1
do
Snake.element[Snake.size-i].x = Snake.element[Snake.size - i - 1].x
Snake.element[Snake.size-i].y = Snake.element[Snake.size - i - 1].y
end

Snake.element[0].x = Snake.element[0].x + offset_x
Snake.element[0].y = Snake.element[0].y + offset_y

if(Snake.element[0].x > 35)
then
Snake.element[0].x = 1
else

if(Snake.element[0].x < 1)
then
Snake.element[0].x = 35
end

end

if(Snake.element[0].y > 16)
then
Snake.element[0].y = 1
else

if(Snake.element[0].y < 1)
then
Snake.element[0].y = 16
end

if(Snake.element[0].x == Snake.food.x and Snake.element[0].y == Snake.food.y)
then
Snake.Enlarge()
Snake.CreateFood()
end

Snake.Collision(Snake.element[0].x,Snake.element[0].y)

end

end