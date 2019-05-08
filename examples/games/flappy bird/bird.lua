
local difficulty_multiplier = {1.0,1.1,1.3,1.5}

Bird = {}

function Bird.new()
    return
    {
        position = {},
        velocity = {},
        lastposition = {},
        speed = {},
        color = 0,
        default_color = 0,
        key = 0,
        stance = 0,
        alt = 0
    }
    end

function Bird.Update(this)

    if(win.KeyDown(this.key) or win.KeyDown(this.alt))
then
    this.stance = 1
    this.velocity.y = this.speed.up * difficulty_multiplier[difficulty + 1]
else
    this.stance = 2
    this.velocity.y = this.speed.down * difficulty_multiplier[difficulty + 1]
end

this.lastposition.x = this.position.x
this.lastposition.y = this.position.y

this.position.x = this.position.x + this.velocity.x
this.position.y = this.position.y + this.velocity.y



end


function Bird.Draw(this)


    conio.SetColor(Image.get(background,math.floor(this.lastposition.x),math.floor(this.lastposition.y)))

    if(this.stance == 1)
    then

floorPosition(this.lastposition.x,this.lastposition.y)
conio.Print("    ")
floorPosition(this.lastposition.x,this.lastposition.y+1)
conio.Print("    ")
conio.SetColor(this.color)
floorPosition(this.position.x+1,this.position.y)
conio.Print("   ")
floorPosition(this.position.x,this.position.y+1)
conio.Print("    ")
    else
        if(this.stance == 2)
        then

            floorPosition(this.lastposition.x,this.lastposition.y)
            conio.Print("    ")
            floorPosition(this.lastposition.x,this.lastposition.y+1)
            conio.Print("    ")
            conio.SetColor(this.color)
            floorPosition(this.position.x,this.position.y)
            conio.Print("    ")
            floorPosition(this.position.x+1,this.position.y+1)
            conio.Print("   ")
       
        else
    
        end
    end


end