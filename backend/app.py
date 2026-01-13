from fastapi import FastAPI, HTTPException
from models import environment
from db import Db

# For now without envars bc im lazy...
db = Db("host=localhost port=5432 dbname=heitorsroom user=postgres password=postgres")

app = FastAPI()

@app.post("/environment")
async def add_env(req: environment.EnvironmentRequestDto) -> str:
    env = environment.reqToEntity(req)
    db.add_environment_info(env)
    return "Env info inserted to DB"

@app.get("/environment/lastmetric")
async def get_metric() -> environment.EnvironmentResponseDto:
    try:
        env = db.get_last_env_info()
    except ValueError:
        raise HTTPException(status_code=404, detail="No environment data found")
    return environment.entityToRes(env)