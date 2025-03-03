FROM gcc:latest

WORKDIR /app

COPY . .

# Compile the server
RUN gcc -o server server.c

# Expose the port the server runs on
EXPOSE 8080

# Command to run the server
CMD ["./server"]