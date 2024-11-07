package main

import (
	"bufio"
	"crypto/sha256"
	"encoding/hex"
	"fmt"
	"net"
	"os"
	"regexp"
	"strings"
)

const charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmn0123456789"
const prefixLen = 5

func getEnv(key string) string {
	val, exists := os.LookupEnv(key)
	if !exists {
		fmt.Printf("Environment variable %s not set\n", key)
		os.Exit(1)
	}
	return val
}

// same principle to find the prefix
func findPrefix(expectedHash string) (string, string, bool) {
	for _, prefix := range generatePrefixes(charset, prefixLen) {
		hash := sha256.Sum256([]byte(prefix))
		computedHash := hex.EncodeToString(hash[:])
		if computedHash == expectedHash {
			return prefix, computedHash, true
		}
	}
	return "", "", false
}

func generatePrefixes(chars string, length int) []string {
	if length == 0 {
		return []string{""}
	}
	subPrefixes := generatePrefixes(chars, length-1)
	var prefixes []string
	for _, ch := range chars {
		for _, sub := range subPrefixes {
			prefixes = append(prefixes, string(ch)+sub)
		}
	}
	return prefixes
}

func solveChallenges(conn net.Conn) {
	reader := bufio.NewReader(conn)
	re := regexp.MustCompile(`sha256\(xxxx\) == ([\w\d]+)`)

	for i := 1; i <= 5; i++ {
		fmt.Printf("\nChallenge %d\n", i)

		//read data
		data, err := reader.ReadString('\n')
		data = strings.TrimSpace(data)

		// check if the hashes are matching 
		match := re.FindStringSubmatch(data)
		expectedHash := match[1]
		fmt.Printf("sha256(xxxx) == %s\n", expectedHash)

		//find hash 
		prefix, computedHash, found := findPrefix(expectedHash)
		if !found {
			fmt.Println("Failed to find a valid prefix within time constraints.")
			continue
		}

		fmt.Printf("   Prefix: '%s'\n", prefix)
		fmt.Printf("   Expected Hash: %s\n", expectedHash)
		fmt.Printf("   Computed Hash: %s\n", computedHash)

		prompt, err := reader.ReadString('\n')
		if err != nil {
			fmt.Println("Error reading mine prompt from server:", err)
			return
		}
		prompt = strings.TrimSpace(prompt)
		if prompt != "Mine xxxx:" {
			fmt.Println("Unexpected server prompt, expected 'Mine xxxx:', got:", prompt)
			continue
		}

		_, err = conn.Write([]byte(prefix + "\n"))
		if err != nil {
			fmt.Println("Error sending data to server:", err)
			return
		}
	}

	var finalResponse strings.Builder
	for {
		line, err := reader.ReadString('\n')
		if err != nil {
			break
		}
		finalResponse.WriteString(line)
	}

	fmt.Println("\n---------------------------------------------------------------")
	fmt.Println(strings.TrimSpace(finalResponse.String()))
	fmt.Println("---------------------------------------------------------------")
}

func main() {
	ip := getEnv("SERVER_IP")
	port := getEnv("SERVER_PORT")
	address := ip + ":" + port

	// connect
	conn, err := net.Dial("tcp", address)
	if err != nil {
		fmt.Printf("Failed to connect to %s: %v\n", address, err)
		return
	}
	defer conn.Close()

	solveChallenges(conn)
}
