
Image = {}

function Image.new(x,y)
return 
{
    xsize = x,
    ysize = y,
    data = malloc(x*y*10)
}
end

function Image.release(this)
free(data)
end

function Image.get(this,x,y)

    return get(this.data,39*x+y)

end

function Image.set(this,x,y,value)

    set(this.data,39*x+y,value)

end