package main

import (
    "fmt"
    "log"
    "net"
    "strings"
    "sync"

    "github.com/mhale/smtpd"// server handling 
)

var (
    messages []string //store messages
    mu       sync.Mutex
)

//handle incomig email
func handleMail(origin net.Addr, from string, to []string, data []byte) error {
    mu.Lock()
    defer mu.Unlock()

    dataStr := string(data)
    lines := strings.Split(dataStr, "\n")

    // filter out the IP
    var filteredLines []string
    for _, line := range lines {
        if !strings.HasPrefix(line, "Received:") {
            filteredLines = append(filteredLines, line)
        }
    }

    message := fmt.Sprintf("From: %s\nTo: %s\nData:\n%s\n", from, to, strings.Join(filteredLines, "\n"))
    messages = append(messages, message)
    fmt.Println("New email received!")
    return nil
}

func displayMessages() {
    mu.Lock()
    defer mu.Unlock()

    if len(messages) == 0 {
        fmt.Println("No messages.")
        return
    }

    fmt.Println("Received Messages:")
    for i, msg := range messages {
        fmt.Printf("[%d] %s\n", i+1, msg)
    }
}

func main() {
    fmt.Println("Starting SMTP server on port 2525...")
    go func() {
        err := smtpd.ListenAndServe("0.0.0.0:2525", handleMail, "Distributed SMTP Server", "")
        if err != nil {
            log.Fatal(err)
        }
    }()

    for {
        var command string
        fmt.Println("Enter 'read' to view messages or 'exit' to quit:")
        fmt.Scanln(&command)

        switch strings.ToLower(command) {
        case "read":
            displayMessages()
        case "exit":
            fmt.Println("Shutting down server...")
            return
        default:
            fmt.Println("Unknown command.")
        }
    }
}
