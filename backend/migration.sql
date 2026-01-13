CREATE TABLE IF NOT EXISTS environment (
    id SERIAL PRIMARY KEY,
    humidity FLOAT NOT NULL,
    temperature FLOAT NOT NULL,
    heatindex FLOAT NOT NULL,
    timestamp TIMESTAMP WITH TIME ZONE NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE INDEX IF NOT EXISTS idx_environment_timestamp ON environment(timestamp DESC);