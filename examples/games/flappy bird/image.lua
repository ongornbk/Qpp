
Image = {}

function Image.new(x,y)
return 
{
    xsize = x,
    ysize = y,
    data = malloc(x*y)
}
end

function Image.release(this)
free(data)
end

function Image.get(this,x,y)
    return get(this.data,(this.xsize)*x+y)
end

function Image.set(this,x,y,value)
    set(this.data,(this.xsize*x)+y,value)
end