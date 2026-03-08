package main

import (
	"bufio"
	"fmt"
	reg "lab2/pkg/regex_pkg"
	"os"
	"strings"
)

func main() {
	reader := bufio.NewReader(os.Stdin)
	fmt.Printf("Enter regex: ")
	input, err := reader.ReadString('\n')
	input = strings.TrimSuffix(input, "\n")
	Tree, err := reg.BuildSyntaxTree(input)
	if Tree == nil{
		fmt.Println(err)
		return
	}
	Tree.Print()
	return
}