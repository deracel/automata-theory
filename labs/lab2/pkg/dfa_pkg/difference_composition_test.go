package dfa_test

import dfa "lab2/pkg/dfa_pkg"

func AreEquivalent(dfa1, dfa2 *dfa.Dfa) bool {
	diff1 := dfa.Difference(dfa1, dfa2)
	diff2 := dfa.Difference(dfa2, dfa1)

	if !isLanguageEmpty(diff1) || !isLanguageEmpty(diff2){
		return false
	}
	return true
}


func isLanguageEmpty(d *dfa.Dfa) bool {
	visited := make(map[*dfa.DfaState]bool)
	queue := []*dfa.DfaState{d.StartState}
	visited[d.StartState] = true

	for len(queue) > 0 {
		current := queue[0]
		queue = queue[1:]

		if current.IsAcceptable {
			return false
		}

		for _, next := range current.Transitions {
			if next != nil && next != d.ErrorState && !visited[next] {
				visited[next] = true
				queue = append(queue, next)
			}
		}
	}
	return true
}


