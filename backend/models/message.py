from datetime import datetime, timezone
from pydantic import BaseModel, Field

class MessageRequestDto(BaseModel):
    message: str 
    username: str 

class Message(MessageRequestDto):
    id: int | None = None
    timestamp: datetime = Field(
        default_factory=lambda: datetime.now(timezone.utc)
    )

class MessageResponseDto(BaseModel):
    timestamp: datetime
    id: int
    message: str
    username: str

def reqToEntity(req: MessageRequestDto) -> Message:
    return Message(message=req.message, username=req.username)

def entityToRes(entity: Message) -> MessageResponseDto:
    if entity.id is None:
        raise ValueError("Message entity must have an id to build a response")
    return MessageResponseDto(
        id=entity.id,
        message=entity.message,
        username=entity.username,
        timestamp=entity.timestamp,
    )