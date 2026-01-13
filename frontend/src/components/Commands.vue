<template>
  <div class="min-h-screen bg-gray-900 p-6">
    <div class="max-w-6xl mx-auto space-y-6">
      <!-- Header -->
      <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
        <div class="flex justify-between items-center">
          <div>
            <h1 class="text-3xl font-bold text-white">Server Commands</h1>
            <p class="text-gray-400 mt-1">Execute administrative commands</p>
          </div>
          <div class="flex space-x-3">
            <Button @click="refreshData" :disabled="loading" variant="default" class="flex items-center space-x-2 bg-blue-600 hover:bg-blue-700">
              <RefreshCwIcon class="h-4 w-4" :class="{ 'animate-spin': loading }" />
              <span>Refresh</span>
            </Button>
          </div>
        </div>
      </div>

      <!-- Overview -->
      <div class="grid grid-cols-1 md:grid-cols-3 gap-6">
        <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
          <div class="flex items-center space-x-3 text-center">
            <GamepadIcon class="h-8 w-8 text-purple-400" />
            <div>
              <p class="text-sm text-gray-400 text-center">Active Lobbies</p>
              <p class="text-2xl font-bold text-white">{{ overview.activeLobbies || 0 }}</p>
            </div>
          </div>
          <div v-if="overview.lobbyDetails.length > 0" class="mt-4">
            <ul class="text-sm text-gray-300 space-y-1">
              <li v-for="lobby in overview.lobbyDetails" :key="lobby">{{ lobby.split(':')[0] }}</li>
            </ul>
          </div>
        </div>

        <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
          <div class="flex items-center space-x-3 text-center">
            <UsersIcon class="h-8 w-8 text-green-400" />
            <div>
              <p class="text-sm text-gray-400 text-center">Logged In Players</p>
              <p class="text-2xl font-bold text-white">{{ overview.loggedInPlayers || 0 }}</p>
            </div>
          </div>
          <div v-if="overview.playerDetails.length > 0" class="mt-4">
            <ul class="text-sm text-gray-300 space-y-1">
              <li v-for="player in overview.playerDetails" :key="player">{{ player }}</li>
            </ul>
          </div>
        </div>

        <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
          <div class="flex items-center space-x-3 text-center">
            <PlayIcon class="h-8 w-8 text-blue-400" />
            <div>
              <p class="text-sm text-gray-400 text-center">Players In Game</p>
              <p class="text-2xl font-bold text-white">{{ overview.playersInGame || 0 }}</p>
            </div>
          </div>
          <div v-if="overview.inGamePlayers.length > 0" class="mt-4">
            <ul class="text-sm text-gray-300 space-y-1">
              <li v-for="player in overview.inGamePlayers" :key="player">{{ player }}</li>
            </ul>
          </div>
        </div>
      </div>

      <!-- Command Prompt -->
      <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
        <h2 class="text-xl font-bold text-white mb-4">Command Prompt</h2>
        <div class="space-y-4">
          <div class="flex space-x-2">
            <input
              v-model="command"
              @keydown.enter="executeCommand"
              type="text"
              placeholder="Enter command..."
              class="flex-1 bg-gray-700 border border-gray-600 rounded-lg px-4 py-2 text-white placeholder-gray-400 focus:outline-none focus:ring-2 focus:ring-indigo-500"
            />
            <Button @click="executeCommand" :disabled="!command.trim()" variant="default" class="bg-blue-600 hover:bg-blue-700">
              Execute
            </Button>
          </div>

          <!-- Suggestions -->
          <div v-if="allSuggestions.length > 0" class="bg-gray-700 rounded-lg p-4">
            <p class="text-sm text-gray-400 mb-2">Suggestions:</p>
            <div class="space-y-1">
              <button
                v-for="suggestion in allSuggestions"
                :key="suggestion"
                @click="command = suggestion"
                class="block w-full text-left text-gray-300 hover:text-white hover:bg-gray-600 px-2 py-1 rounded"
              >
                {{ suggestion }}
              </button>
            </div>
          </div>

          <!-- Output -->
          <div v-if="output" class="bg-gray-700 rounded-lg p-4">
            <p class="text-sm text-gray-400 mb-2">Output:</p>
            <p class="text-white">{{ output }}</p>
          </div>
        </div>
      </div>

      <!-- Auto-refresh indicator -->
      <div class="fixed bottom-0 left-1/2 transform -translate-x-1/2 text-gray-500 text-sm bg-gray-900 px-4 py-2 rounded-t">
        Auto-refreshing every 2 seconds • Last updated: {{ lastUpdate }}
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted, onUnmounted } from 'vue'
import Button from './ui/Button.vue'
import { GamepadIcon, UsersIcon, PlayIcon, RefreshCwIcon } from 'lucide-vue-next'

interface Props {
  password: string
}

const props = defineProps<Props>()

const command = ref('')
const allSuggestions = ref<string[]>([])
const output = ref('')
const lastUpdate = ref('')
const loading = ref(false)
const overview = ref({
  activeLobbies: 0,
  loggedInPlayers: 0,
  playersInGame: 0,
  lobbyDetails: [] as string[],
  playerDetails: [] as string[],
  inGamePlayers: [] as string[]
})
const refreshInterval = ref<NodeJS.Timeout | null>(null)

const fetchOverview = async () => {
  loading.value = true
  try {
    const response = await fetch('/api/info', {
      headers: {
        'Authorization': `Bearer ${props.password}`
      }
    })
    if (response.ok) {
      const data = await response.json()
      overview.value = {
        activeLobbies: data.activeLobbies || 0,
        loggedInPlayers: data.playerDetails ? data.playerDetails.length : 0,
        playersInGame: data.inGamePlayers ? data.inGamePlayers.length : 0,
        lobbyDetails: data.lobbyDetails || [],
        playerDetails: data.playerDetails || [],
        inGamePlayers: data.inGamePlayers || []
      }
      updateLastUpdate()
    }
  } catch (error) {
    console.error('Error fetching overview:', error)
  } finally {
    loading.value = false
  }
}

const fetchSuggestions = async () => {
  try {
    const response = await fetch('/api/commands/suggestions', {
      headers: {
        'Authorization': `Bearer ${props.password}`
      }
    })
    if (response.ok) {
      const data = await response.json()
      allSuggestions.value = data.suggestions || []
    }
  } catch (error) {
    console.error('Error fetching suggestions:', error)
  }
}

const updateLastUpdate = () => {
  lastUpdate.value = new Date().toLocaleTimeString()
}

const refreshData = () => {
  fetchOverview()
}

const executeCommand = async () => {
  if (!command.value.trim()) return

  try {
    const response = await fetch('/api/commands/execute', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
        'Authorization': `Bearer ${props.password}`
      },
      body: JSON.stringify({ command: command.value })
    })

    const data = await response.json()
    output.value = data.message || 'Command executed'

    if (response.ok) {
      // Refresh overview after command execution
      await fetchOverview()
    }
  } catch (error) {
    output.value = 'Error executing command'
    console.error('Error executing command:', error)
  }

  command.value = ''
}

onMounted(() => {
  fetchOverview()
  fetchSuggestions()
  refreshInterval.value = setInterval(fetchOverview, 2000)
})

onUnmounted(() => {
  if (refreshInterval.value) {
    clearInterval(refreshInterval.value)
  }
})
</script>