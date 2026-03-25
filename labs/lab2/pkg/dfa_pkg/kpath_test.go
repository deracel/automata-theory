package dfa_test

import (
	dfa "lab2/pkg/dfa_pkg"
	"testing"
)

func TestSimpleKpath(t *testing.T) {
	tests := []struct{
		startExpr string
	}{
		{
			startExpr: "abc",
		},
		{
			startExpr: "a...b...c...d...",
		},
		{
			startExpr: "a?b?c?d?f?q?",
		},
		{
			startExpr: "a|bc|d|qw|yui|q",
		},
		{
			startExpr: "a{3}b{0}c{10}",
		},
	}
	for _, test := range tests {
		t.Run(test.startExpr, func(t *testing.T) {
			df, _, err := dfa.BuildDfa(test.startExpr)
			if err != nil {
				t.Fatalf("failed to build dfa: %v", err)
			}
			resultExpr, err := dfa.BuildRegexFromDFA(df)
			if err != nil {
				t.Fatalf("failed to build regex: %v", err)
			}
			otherDfa, _, err := dfa.BuildDfa(resultExpr)
			if err != nil {
				t.Fatalf("failed to build dfa: %v", err)
			}
			if dfa.AreEquivalent(df, otherDfa) {
				return
			}
			t.Fatalf("dfa are not equivalent")
		})
	}
}