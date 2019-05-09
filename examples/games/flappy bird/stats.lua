import("sql")

function Initialize()

    local sqlite = sql.Open("stats.db")

    sql.Exec(sqlite,"create table stats (Score smallint, Difficulty varchar(30), Date datetime)")
    
    sql.Close(sqlite)

    sqlite = nil

end

function InsertRecord(score,diff)

    local sqlite = sql.Open("stats.db")

    sql.Exec(sqlite,"insert into stats (Score, Difficulty, Date) values (" .. score ..", '" .. diff .."', datetime('now'))")
    
    sql.Close(sqlite)

    sqlite = nil

end

function PrintRecords()

local sqlite = sql.Open("stats.db")

conio.Gotoxy(0,0)
system.System("cls")
conio.SetColor(11)

conio.Println("Stats:")

local count = sql.Exec(sqlite,"select * from stats order by score desc limit 10")

for i = 0,count-1,1
do
conio.Print(sql.Extract())
end

end