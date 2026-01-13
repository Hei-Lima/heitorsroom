import psycopg
from models import environment


class Db():
    conn: psycopg.Connection

    def __init__(self, connection_string: str):
        self.conn = psycopg.connect(connection_string)

    # I really do know that this logic could be part of the models... But im lazy! Too bad.
    def add_environment_info(self, env: environment.Environment) -> None:
        query = "INSERT INTO environment (humidity, temperature, heatindex, timestamp) VALUES (%s, %s, %s, %s)"
        try:
            with self.conn.cursor() as cur:
                cur.execute(query, (env.humidity, env.temperature, env.heatindex, env.timestamp))
            self.conn.commit()
        except Exception:
            self.conn.rollback()
            raise

    def get_last_env_info(self) -> environment.Environment:
        query = "SELECT Id, humidity, temperature, heatindex, timestamp FROM environment ORDER BY timestamp DESC LIMIT 1"
        try:
            with self.conn.cursor() as cur:
                cur.execute(query)
                qry = cur.fetchone()
                if qry is None:
                    raise ValueError("No environment data found")
                env = environment.Environment(
                    id=qry[0],
                    humidity=qry[1],
                    temperature=qry[2],
                    heatindex=qry[3],
                    timestamp=qry[4]
                )
                return env
        except Exception:
            self.conn.rollback()
            raise
        
    def close(self):
        self.conn.close()

