FROM gcc:latest

WORKDIR /app

# Copy all files explicitly
COPY Server.c .
COPY Client.c .

# Debug - list files to verify they exist
RUN ls -la

# Compile the server
RUN gcc -o server server.c -Wall

# Expose the port the server runs on
EXPOSE 8080

# Command to run the server
CMD ["./server"]