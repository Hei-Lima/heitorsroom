from fastapi import FastAPI, HTTPException, Depends
from fastapi.security import HTTPBearer, HTTPAuthorizationCredentials
from models import environment
from db import Db
import dotenv
import os

dotenv.load_dotenv()

db_host = os.getenv("DB_HOST")
db_port = os.getenv("DB_PORT")
db_name = os.getenv("DB_NAME")
db_user = os.getenv("DB_USER")
db_password = os.getenv("DB_PASSWORD")
api_token = os.getenv("API_TOKEN")

db = Db(f"host={db_host} port={db_port} dbname={db_name} user={db_user} password={db_password}")

app = FastAPI()
security = HTTPBearer()

def verify_token(credentials: HTTPAuthorizationCredentials = Depends(security)):
    if credentials.credentials != api_token:
        raise HTTPException(status_code=401, detail="This route is meant only for my ESP32... Nice try...")
    return credentials.credentials

@app.post("/environment")
async def add_env(req: environment.EnvironmentRequestDto, token: str = Depends(verify_token)) -> str:
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