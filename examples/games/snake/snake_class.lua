Snake = {}

function Snake.new()
return
{
    element = {},
    size = 0,
    velocity = {},
    lastposition = {},
    color = 0,
    key = {}
}
end

function Snake.Grow(snake)
    snake.size = snake.size + 1
    snake.element[snake.size] = {}
    snake.element[snake.size].x = snake.element[snake.size-1].x
    snake.element[snake.size].y = snake.element[snake.size-1].y
end

function Snake.Collision(snake,x,y)

    local collision = 0
for i = 1,snake.size,1
do

    if(x == snake.element[i].x and y == snake.element[i].y)
    then
        collision = i
        break;
    end
end

if(collision > 0)
then
for i = collision,snake.size,1
do
   map.Print(snake.element[i].x,snake.element[i].y," ")
end

snake.size = collision

end

end

function Snake.Draw(snake)

    con.SetColor(snake.color)
    map.Print(snake.element[0].x,snake.element[0].y,"@")

for i = 1,snake.size-1,1
do
    local char = tostring(i%10)

    if(snake.element[i].x == map.food.x and snake.element[i].y == map.food.y)
    then
      char = "X"
    end
    map.Print(snake.element[i].x,snake.element[i].y,char)
end

    if(snake.size > 0)
    then
    map.Print(snake.element[snake.size].x,snake.element[snake.size].y,"*")
    end

    map.Print(snake.lastposition.x,snake.lastposition.y," ")
end

function Snake.Update(snake)

    if(win.KeyDown(snake.key[0]))
then
    snake.velocity.x = 0
    snake.velocity.y = -1
else

    if(win.KeyDown(snake.key[1]))
    then
        snake.velocity.x = 1
        snake.velocity.y = 0
    else

        if(win.KeyDown(snake.key[2]))
        then
            snake.velocity.x = -1
            snake.velocity.y = 0
        else

            if(win.KeyDown(snake.key[3]))
            then
                snake.velocity.x = 0
                snake.velocity.y = 1
            end

        end

    end

end

snake.lastposition.x = snake.element[snake.size].x
snake.lastposition.y = snake.element[snake.size].y

for i = 0,snake.size-1,1
do
    snake.element[snake.size-i].x = snake.element[snake.size - i - 1].x
    snake.element[snake.size-i].y = snake.element[snake.size - i - 1].y
end

snake.element[0].x = snake.element[0].x + snake.velocity.x
snake.element[0].y = snake.element[0].y + snake.velocity.y

if(snake.element[0].x > map.width)
then
    snake.element[0].x = 1
else

if(snake.element[0].x < 1)
then
    snake.element[0].x = map.width
end

end

if(snake.element[0].y > map.height)
then
    snake.element[0].y = 1
else

if(snake.element[0].y < 1)
then
    snake.element[0].y = map.height
end

if(snake.element[0].x == map.food.x and snake.element[0].y == map.food.y)
then

con.SetColor(7)
map.Print(0,map.food.y,"|")
map.Print(map.food.x,0,"-")
map.Print(map.width+1,map.food.y,"|")
map.Print(map.food.x,map.height+1,"-")

Snake.Grow(snake)
map.CreateFood()

map.Print(0,map.food.y,"+")
map.Print(map.food.x,0,"+")
map.Print(map.width+1,map.food.y,"+")
map.Print(map.food.x,map.height+1,"+")
end

Snake.Collision(snake,snake.element[0].x,snake.element[0].y)
--SnakeRed.Collision(SnakeBlue.element[0].x,SnakeBlue.element[0].y)
end

end