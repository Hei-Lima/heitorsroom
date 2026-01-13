from datetime import datetime, timezone
from pydantic import BaseModel, Field

class EnvironmentRequestDto(BaseModel):
    humidity: float 
    temperature: float 
    heatindex: float

class Environment(EnvironmentRequestDto):
    id: int | None = None #bc the DB will generate it automatically
    timestamp: datetime = Field(
        default_factory=lambda: datetime.now(timezone.utc)
    )

class EnvironmentResponseDto(EnvironmentRequestDto):
    timestamp: datetime
    id: int

def reqToEntity(req: EnvironmentRequestDto) -> Environment:
    return Environment(humidity=req.humidity, temperature=req.temperature, heatindex=req.heatindex)

def entityToRes(entity: Environment) -> EnvironmentResponseDto:
    if entity.id is None:
        raise ValueError("Environment entity must have an id to build a response")
    return EnvironmentResponseDto(
        id=entity.id,
        humidity=entity.humidity,
        temperature=entity.temperature,
        heatindex=entity.heatindex,
        timestamp=entity.timestamp,
    )