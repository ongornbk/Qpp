
local difficulty_multiplier = {0.8,1.0,1.0,1.1,1.1,1.2}

Bird = {}

function Bird.new()
    return
    {
        position = {},
        velocity = {},
        lastposition = {},
        speed = {},
        color = 0,
        darkcolor = 16,
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
conio.FWrite("    ",4)
floorPosition(this.lastposition.x,this.lastposition.y+1)
conio.FWrite("    ",4)
conio.SetColor(this.darkcolor)
floorPosition(this.position.x+1,this.position.y)
conio.FWrite("   ",3)
conio.SetColor(this.color)
floorPosition(this.position.x,this.position.y+1)
conio.FWrite("   ",3)
    else
        if(this.stance == 2)
        then

            floorPosition(this.lastposition.x,this.lastposition.y)
            conio.FWrite("    ",4)
            floorPosition(this.lastposition.x,this.lastposition.y+1)
            conio.FWrite("    ",4)
            conio.SetColor(this.color)
            floorPosition(this.position.x,this.position.y)
            conio.FWrite("   ",3)
            conio.SetColor(this.darkcolor)
            floorPosition(this.position.x+1,this.position.y+1)
            conio.FWrite("   ",3)
       
        else
    
        end
    end


end