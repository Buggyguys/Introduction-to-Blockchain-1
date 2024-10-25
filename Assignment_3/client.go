package main

import (
    "bufio"
    "fmt"
    "log"
    "net/smtp"
    "os"
)

func main() {
    fmt.Println("Write your message and press enter on an empty line to send it.")
    scanner := bufio.NewScanner(os.Stdin)
    body := ""

    for scanner.Scan() {
        line := scanner.Text()
        if line == "" {
            break
        }
        body += line + "\n"
    }
    if err := scanner.Err(); err != nil {
        log.Fatalf("Error reading input: %v", err)
    }

    //using OS environment variables
    from := os.Getenv("EMAIL_FROM")
    to := os.Getenv("EMAIL_TO")
    smtpServer := os.Getenv("SMTP_SERVER")
    subject := "Reply Email"

    if from == "" || to == "" || smtpServer == "" {
        log.Fatal("Set env vars")
    }

    msg := fmt.Sprintf("From: %s\nTo: %s\nSubject: %s\n\n%s", from, to, subject, body)

    // Send
    err := smtp.SendMail(smtpServer, nil, from, []string{to}, []byte(msg))
    if err != nil {
        log.Fatal(err)
    }

    fmt.Println("Email sent!")
}
