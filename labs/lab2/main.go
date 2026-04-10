package main

import (
	"fmt"
	dfa "lab2/pkg/dfa_pkg"
	dia "lab2/pkg/dialogue_pkg"
	graph "lab2/pkg/graph_pkg"
)

func main() {
	auto1, _, _ := dfa.BuildDfa("a")
	auto2, _, _ := dfa.BuildDfa("a")
	auto3 := dfa.Difference(auto1, auto2)
	auto10 := dfa.Difference(auto3, auto3)
	err := graph.SaveAndOpenGraphVizDfa(auto10, "empty1.dot")
	reg, _ := dfa.BuildRegexFromDFA(auto3)
	fmt.Println(reg)
	auto4, _, _ := dfa.BuildDfa(reg)
	err = graph.SaveAndOpenGraphVizDfa(auto4, "test.dot")
	if err != nil {
		return 
	}
	dia.Dialogue()
	return
}

