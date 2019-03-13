
function iow(str)
AppendStringToFile(str)
end

function iowl(str)
AppendStringToFile(str .. "\n")
end

function UnpackItem(item)

title1,title2 = string.find(item,"dn=")
title3,title4 = string.find(item,"&tr=")
local title = string.sub(item,title2+1,title3-1)
title = string.gsub(title,"+"," ")
iowl("<tr>")
iowl("<td>")
iowl(tostring(title))
iowl("</td>")
iowl("<td>")
iow("<a href=")
iow("\"magnet:?xt=urn:btih:")
iowl(item)
iowl("\">")
iowl("Copy Link!")
iowl("</a>")
iowl("</td>")

iowl("</tr>")
end

function UnpackResult(result)
OpenFileAppend("magnets.html")

iowl("<html>")
iowl("<head>")
iowl("<link rel=\"stylesheet\" href=\"style.css\">")
iowl("<title>Szrot Downloader</title>")
iowl("</head>")
iowl("<body>")
iowl("<table>")
iowl("<tr>")
iowl("<th>Title</th>")
iowl("<th>Link</th>")
iowl("</tr>")




local guard = true

while(guard == true)
do
local _beg1,_beg2 = string.find(result, "btih:")
if(_beg1 == nil)
then
guard = false
else

result = string.sub(result, _beg1 + 5)
local _end = string.find(result,"title=")
_end = _end - 3
local item = string.sub(result,1,_end)
UnpackItem(tostring(item))
end


end
iowl("</table>")
iowl("</body>")
iowl("</html>")
CloseFile()
end

