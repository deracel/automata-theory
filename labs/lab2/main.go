package main

import (
	"bufio"
	"fmt"
	reg "lab2/pkg/regex_pkg"
	"os"
	"strings"
	nfa "lab2/pkg/nfa_pkg"
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
	fmt.Println("\n\n")
	Nfa := nfa.BuildNfaFromTree(Tree)
	Nfa.Print()
	Nfa.SaveAndOpenGraphViz("graphs/graph.dot")
	return
}